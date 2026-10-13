#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <random>
#include <cstring>
#include "../vendor/nlohmann/json.hpp"

namespace alumniverse {

class CryptoUtils {
public:
    // Pure C++ standard FIPS 180-2 SHA-256 implementation
    static std::string sha256(const std::string& input) {
        uint32_t h[8] = {
            0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
            0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
        };

        const uint32_t k[64] = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
            0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
            0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
        };

        std::vector<uint8_t> data(input.begin(), input.end());
        uint64_t bitLen = data.size() * 8;

        data.push_back(0x80);
        while ((data.size() % 64) != 56) {
            data.push_back(0x00);
        }

        for (int i = 7; i >= 0; --i) {
            data.push_back((bitLen >> (i * 8)) & 0xff);
        }

        auto rotr = [](uint32_t x, uint32_t n) { return (x >> n) | (x << (32 - n)); };

        for (size_t chunk = 0; chunk < data.size(); chunk += 64) {
            uint32_t w[64];
            for (size_t i = 0; i < 16; ++i) {
                w[i] = (data[chunk + i * 4] << 24) |
                       (data[chunk + i * 4 + 1] << 16) |
                       (data[chunk + i * 4 + 2] << 8) |
                       (data[chunk + i * 4 + 3]);
            }
            for (size_t i = 16; i < 64; ++i) {
                uint32_t s0 = rotr(w[i - 15], 7) ^ rotr(w[i - 15], 18) ^ (w[i - 15] >> 3);
                uint32_t s1 = rotr(w[i - 2], 17) ^ rotr(w[i - 2], 19) ^ (w[i - 2] >> 10);
                w[i] = w[i - 16] + s0 + w[i - 7] + s1;
            }

            uint32_t a = h[0], b = h[1], c = h[2], d = h[3];
            uint32_t e = h[4], f = h[5], g = h[6], h_val = h[7];

            for (size_t i = 0; i < 64; ++i) {
                uint32_t s1 = rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25);
                uint32_t ch = (e & f) ^ ((~e) & g);
                uint32_t temp1 = h_val + s1 + ch + k[i] + w[i];
                uint32_t s0 = rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22);
                uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
                uint32_t temp2 = s0 + maj;

                h_val = g;
                g = f;
                f = e;
                e = d + temp1;
                d = c;
                c = b;
                b = a;
                a = temp1 + temp2;
            }

            h[0] += a; h[1] += b; h[2] += c; h[3] += d;
            h[4] += e; h[5] += f; h[6] += g; h[7] += h_val;
        }

        std::ostringstream oss;
        for (int i = 0; i < 8; ++i) {
            oss << std::hex << std::setw(8) << std::setfill('0') << h[i];
        }
        return oss.str();
    }

    static std::string hmacSha256(const std::string& key, const std::string& data) {
        std::string k = key;
        if (k.size() > 64) {
            k = sha256(k);
        }
        k.resize(64, '\0');

        std::string o_key_pad(64, '\0');
        std::string i_key_pad(64, '\0');
        for (int i = 0; i < 64; ++i) {
            o_key_pad[i] = k[i] ^ 0x5c;
            i_key_pad[i] = k[i] ^ 0x36;
        }

        std::string innerHash = sha256(i_key_pad + data);
        return sha256(o_key_pad + innerHash);
    }

    static std::string generateSalt(size_t length = 16) {
        static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);

        std::string s;
        for (size_t i = 0; i < length; ++i) {
            s += alphanum[dis(gen)];
        }
        return s;
    }

    /**
     * @brief Creates a salted SHA-256 hash formatted as salt$hash.
     */
    static std::string hashPassword(const std::string& password) {
        std::string salt = generateSalt(12);
        std::string hash = sha256(password + salt);
        return salt + "$" + hash;
    }

    /**
     * @brief Constant-time password verification to prevent timing attacks.
     */
    static bool verifyPassword(const std::string& password, const std::string& storedHash) {
        auto pos = storedHash.find('$');
        if (pos == std::string::npos) return false;

        std::string salt = storedHash.substr(0, pos);
        std::string expectedHash = storedHash.substr(pos + 1);
        std::string actualHash = sha256(password + salt);

        if (actualHash.length() != expectedHash.length()) return false;

        // Constant time comparison
        int diff = 0;
        for (size_t i = 0; i < actualHash.length(); ++i) {
            diff |= (actualHash[i] ^ expectedHash[i]);
        }
        return (diff == 0);
    }

    static std::string base64UrlEncode(const std::string& in) {
        static const std::string b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        std::string out;
        int val = 0, valb = -6;
        for (uint8_t c : in) {
            val = (val << 8) + c;
            valb += 8;
            while (valb >= 0) {
                out.push_back(b64[(val >> valb) & 0x3F]);
                valb -= 6;
            }
        }
        if (valb > -6) out.push_back(b64[((val << 8) >> (valb + 8)) & 0x3F]);

        // Replace '+' with '-', '/' with '_', and strip '=' padding
        for (char& c : out) {
            if (c == '+') c = '-';
            else if (c == '/') c = '_';
        }
        out.erase(std::remove(out.begin(), out.end(), '='), out.end());
        return out;
    }

    static std::string base64UrlDecode(const std::string& in) {
        static const std::string b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        std::string copy = in;
        for (char& c : copy) {
            if (c == '-') c = '+';
            else if (c == '_') c = '/';
        }
        while (copy.size() % 4 != 0) copy.push_back('=');

        std::string out;
        std::vector<int> T(256, -1);
        for (int i = 0; i < 64; i++) T[b64[i]] = i;

        int val = 0, valb = -8;
        for (uint8_t c : copy) {
            if (T[c] == -1) break;
            val = (val << 6) + T[c];
            valb += 6;
            if (valb >= 0) {
                out.push_back(char((val >> valb) & 0xFF));
                valb -= 8;
            }
        }
        return out;
    }

    /**
     * @brief Generates a secure HMAC-SHA256 JWT Token with user identity and role.
     */
    static std::string generateJwt(int userId, const std::string& email, const std::string& role, const std::string& secretKey, int expiresInSeconds = 86400) {
        auto now = std::chrono::system_clock::now();
        int64_t exp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() + expiresInSeconds;

        nlohmann::json header = {{"alg", "HS256"}, {"typ", "JWT"}};
        nlohmann::json payload = {
            {"userId", userId},
            {"email", email},
            {"role", role},
            {"exp", exp}
        };

        std::string hB64 = base64UrlEncode(header.dump());
        std::string pB64 = base64UrlEncode(payload.dump());
        std::string toSign = hB64 + "." + pB64;
        std::string sig = base64UrlEncode(hmacSha256(secretKey, toSign));

        return toSign + "." + sig;
    }

    /**
     * @brief Validates JWT Token, checks expiration and signature, and returns payload.
     */
    static std::optional<nlohmann::json> verifyJwt(const std::string& token, const std::string& secretKey) {
        size_t p1 = token.find('.');
        if (p1 == std::string::npos) return std::nullopt;
        size_t p2 = token.find('.', p1 + 1);
        if (p2 == std::string::npos) return std::nullopt;

        std::string hB64 = token.substr(0, p1);
        std::string pB64 = token.substr(p1 + 1, p2 - p1 - 1);
        std::string sigB64 = token.substr(p2 + 1);

        std::string toSign = hB64 + "." + pB64;
        std::string expectedSig = base64UrlEncode(hmacSha256(secretKey, toSign));

        if (sigB64 != expectedSig) return std::nullopt;

        try {
            std::string payloadStr = base64UrlDecode(pB64);
            auto payload = nlohmann::json::parse(payloadStr);

            int64_t exp = payload.value("exp", 0LL);
            auto now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            if (now > exp) {
                return std::nullopt; // Expired
            }
            return payload;
        } catch (...) {
            return std::nullopt;
        }
    }
};

} // namespace alumniverse
