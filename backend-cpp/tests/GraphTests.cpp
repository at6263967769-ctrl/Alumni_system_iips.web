#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include "../include/graph/AlumniGraph.hpp"
#include "../include/graph/AlumniTree.hpp"
#include "../include/repositories/DataRepository.hpp"
#include "../include/services/GraphService.hpp"
#include "../include/services/TreeService.hpp"
#include "../include/utils/CryptoUtils.hpp"
#include "../include/utils/CsvUtils.hpp"

using namespace alumniverse;

int g_passedTests = 0;
int g_failedTests = 0;

#define TEST_ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "[-] FAILED: " << msg << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
            g_failedTests++; \
        } else { \
            std::cout << "[+] PASSED: " << msg << "\n"; \
            g_passedTests++; \
        } \
    } while(0)

void testEmptyGraph() {
    std::cout << "\n--- Testing Empty Graph ---\n";
    AlumniGraph graph;
    TEST_ASSERT(graph.getVertexCount() == 0, "Vertex count is 0 on empty graph");
    TEST_ASSERT(graph.getEdgeCount() == 0, "Edge count is 0 on empty graph");
    TEST_ASSERT(!graph.hasVertex(1), "hasVertex returns false for non-existent node");
    TEST_ASSERT(graph.bfs(1).empty(), "BFS returns empty vector on non-existent node");
    TEST_ASSERT(graph.dfs(1).empty(), "DFS returns empty vector on non-existent node");
    TEST_ASSERT(graph.findShortestPath(1, 2).empty(), "findShortestPath returns empty vector on empty graph");
}

void testSingleVertex() {
    std::cout << "\n--- Testing Single Vertex ---\n";
    AlumniGraph graph;
    GraphNode node;
    node.id = 101;
    node.name = "Alice Test";
    node.currentCompany = "Google";
    graph.addVertex(node);

    TEST_ASSERT(graph.getVertexCount() == 1, "Vertex count is 1 after addition");
    TEST_ASSERT(graph.hasVertex(101), "hasVertex(101) is true");
    auto v = graph.getVertex(101);
    TEST_ASSERT(v.has_value() && v->name == "Alice Test", "getVertex returns correct node data");

    auto bfsResult = graph.bfs(101);
    TEST_ASSERT(bfsResult.size() == 1 && bfsResult[0] == 101, "BFS on single vertex returns [101]");

    auto dfsResult = graph.dfs(101);
    TEST_ASSERT(dfsResult.size() == 1 && dfsResult[0] == 101, "DFS on single vertex returns [101]");

    auto path = graph.findShortestPath(101, 101);
    TEST_ASSERT(path.size() == 1 && path[0] == 101, "Path from vertex to itself returns [101]");

    TEST_ASSERT(graph.removeVertex(101), "removeVertex succeeds");
    TEST_ASSERT(graph.getVertexCount() == 0, "Vertex count is 0 after removal");
    TEST_ASSERT(!graph.hasVertex(101), "hasVertex is false after removal");
}

void testDuplicateAndInvalidEdges() {
    std::cout << "\n--- Testing Edge Validation & Duplicate Handling ---\n";
    AlumniGraph graph;
    GraphNode n1{1, "Node 1"};
    GraphNode n2{2, "Node 2"};
    graph.addVertex(n1);
    graph.addVertex(n2);

    TEST_ASSERT(!graph.addEdge(1, 999, "classmate"), "Cannot add edge to non-existent vertex 999");
    TEST_ASSERT(!graph.addEdge(1, 1, "classmate"), "Self loop edge rejected");

    bool added1 = graph.addEdge(1, 2, "classmate");
    TEST_ASSERT(added1, "Valid edge (1 <-> 2) added successfully");
    TEST_ASSERT(graph.getEdgeCount() == 1, "Edge count is 1");

    bool addedDuplicate = graph.addEdge(1, 2, "classmate");
    TEST_ASSERT(!addedDuplicate, "Duplicate edge correctly rejected");
    TEST_ASSERT(graph.getEdgeCount() == 1, "Edge count remains 1 after duplicate rejection");

    auto neighbors1 = graph.getNeighbors(1);
    TEST_ASSERT(neighbors1.size() == 1 && neighbors1[0].targetId == 2, "Vertex 1 has neighbor 2");
    auto neighbors2 = graph.getNeighbors(2);
    TEST_ASSERT(neighbors2.size() == 1 && neighbors2[0].targetId == 1, "Vertex 2 has neighbor 1");
}

void testDisconnectedComponents() {
    std::cout << "\n--- Testing Disconnected Components ---\n";
    AlumniGraph graph;
    // Component 1: 1 - 2 - 3
    graph.addVertex({1, "A"});
    graph.addVertex({2, "B"});
    graph.addVertex({3, "C"});
    graph.addEdge(1, 2, "colleague");
    graph.addEdge(2, 3, "colleague");

    // Component 2: 4 - 5
    graph.addVertex({4, "D"});
    graph.addVertex({5, "E"});
    graph.addEdge(4, 5, "colleague");

    auto bfsComp1 = graph.bfs(1);
    TEST_ASSERT(bfsComp1.size() == 3, "BFS from node 1 visits exactly 3 nodes in component 1");

    auto bfsComp2 = graph.bfs(4);
    TEST_ASSERT(bfsComp2.size() == 2, "BFS from node 4 visits exactly 2 nodes in component 2");

    auto pathAcross = graph.findShortestPath(1, 4);
    TEST_ASSERT(pathAcross.empty(), "Shortest path between disconnected components is empty");
}

void testCyclicGraphTraversalAndShortestPath() {
    std::cout << "\n--- Testing Cyclic Traversal & Shortest Path ---\n";
    AlumniGraph graph;
    // Cycle: 1 - 2 - 3 - 4 - 1
    // Chord: 1 - 3 (shortcut between 1 and 3)
    graph.addVertex({1, "Node 1"});
    graph.addVertex({2, "Node 2"});
    graph.addVertex({3, "Node 3"});
    graph.addVertex({4, "Node 4"});
    graph.addVertex({5, "Node 5"}); // 3 - 5

    graph.addEdge(1, 2, "friend");
    graph.addEdge(2, 3, "friend");
    graph.addEdge(3, 4, "friend");
    graph.addEdge(4, 1, "friend");
    graph.addEdge(1, 3, "colleague"); // Shortcut
    graph.addEdge(3, 5, "mentor");

    // BFS should terminate safely despite cycle
    auto bfsResult = graph.bfs(1);
    TEST_ASSERT(bfsResult.size() == 5, "BFS terminates safely on cyclic graph and visits all 5 nodes");

    // DFS should terminate safely despite cycle
    auto dfsResult = graph.dfs(1);
    TEST_ASSERT(dfsResult.size() == 5, "DFS terminates safely on cyclic graph and visits all 5 nodes");

    // Shortest path 1 -> 5: Direct is 1 -> 3 -> 5 (length 2 edges) instead of 1 -> 2 -> 3 -> 5
    auto path = graph.findShortestPath(1, 5);
    TEST_ASSERT(path.size() == 3, "Shortest path 1 -> 5 has 3 nodes (length 2 edges)");
    if (path.size() == 3) {
        TEST_ASSERT(path[0] == 1 && path[1] == 3 && path[2] == 5, "Shortest path correctly chooses direct chord [1 -> 3 -> 5]");
    }
}

void testTreeSubtreeExtractionAndPruning() {
    std::cout << "\n--- Testing Hierarchical Tree & Subtree Pruning ---\n";
    Institution inst{1, "IIPS DAVV", "https://iips.edu.in", ""};
    std::vector<Department> depts = {
        {1, 1, "Computer Applications"},
        {2, 1, "Management Studies"}
    };
    std::vector<Batch> batches = {
        {1, 1, "MCA", 2015, 2020},
        {2, 1, "BCA", 2019, 2022},
        {3, 2, "MBA", 2016, 2021}
    };
    std::vector<Alumni> alumniList;
    Alumni a1;
    a1.id = 1; a1.batchId = 1; a1.fullName = "Aarav Sharma"; a1.currentCompany = "Google";
    a1.skills = {"C++", "Distributed Systems"}; a1.verificationStatus = "verified";
    alumniList.push_back(a1);

    Alumni a2;
    a2.id = 2; a2.batchId = 1; a2.fullName = "Priya Patel"; a2.currentCompany = "Microsoft";
    a2.skills = {"C++", "Cloud"}; a2.verificationStatus = "verified";
    alumniList.push_back(a2);

    Alumni a3;
    a3.id = 3; a3.batchId = 3; a3.fullName = "Sneha Kapoor"; a3.currentCompany = "Goldman Sachs";
    a3.skills = {"Financial Modeling"}; a3.verificationStatus = "verified";
    alumniList.push_back(a3);

    AlumniTree tree;
    tree.buildAcademicTree(inst, depts, batches, alumniList);

    auto root = tree.getRoot();
    TEST_ASSERT(root != nullptr, "Tree root is generated");
    TEST_ASSERT(root->alumniCount == 3, "Root alumni count is 3");

    // Subtree of Department 1 (Computer Applications)
    auto dept1Subtree = tree.getSubtree("dept-1");
    TEST_ASSERT(dept1Subtree != nullptr, "Subtree dept-1 found");
    TEST_ASSERT(dept1Subtree->alumniCount == 2, "dept-1 has 2 alumni");

    // Filter by company "Google"
    FilterCriteria filter;
    filter.company = "Google";
    auto filteredTree = tree.filterTree(filter);
    TEST_ASSERT(filteredTree != nullptr, "Filtered tree exists");
    TEST_ASSERT(filteredTree->alumniCount == 1, "Filtered tree contains 1 matching alumni (Google)");

    // Filter by non-existent company
    FilterCriteria filterEmpty;
    filterEmpty.company = "NonExistentCompanyXYZ";
    auto emptyTree = tree.filterTree(filterEmpty);
    TEST_ASSERT(emptyTree == nullptr, "Filtering with 0 matches prunes entire tree cleanly to null");
}

void testCryptoSecurity() {
    std::cout << "\n--- Testing Password Hashing & JWT Security ---\n";
    std::string password = "SecretPassword123!";
    std::string hash = CryptoUtils::hashPassword(password);

    TEST_ASSERT(!hash.empty(), "Password hash generated");
    TEST_ASSERT(hash.find('$') != std::string::npos, "Hash format contains salt delimiter ($)");
    TEST_ASSERT(CryptoUtils::verifyPassword(password, hash), "Password verification succeeds with correct password");
    TEST_ASSERT(!CryptoUtils::verifyPassword("WrongPassword", hash), "Password verification fails with incorrect password");

    std::string secret = "test_super_secret_jwt_key_123456";
    std::string token = CryptoUtils::generateJwt(42, "user@test.com", "admin", secret, 3600);
    TEST_ASSERT(!token.empty(), "JWT token generated");

    auto payload = CryptoUtils::verifyJwt(token, secret);
    TEST_ASSERT(payload.has_value(), "Valid JWT verified successfully");
    if (payload.has_value()) {
        TEST_ASSERT(payload.value()["userId"] == 42, "JWT payload contains correct userId");
        TEST_ASSERT(payload.value()["role"] == "admin", "JWT payload contains correct role");
    }

    auto invalidPayload = CryptoUtils::verifyJwt(token, "wrong_secret_key");
    TEST_ASSERT(!invalidPayload.has_value(), "JWT with wrong secret key fails verification");
}

void testCsvParsingAndSanitization() {
    std::cout << "\n--- Testing CSV Parser & Formula Injection Protection ---\n";
    std::string csv = "full_name,graduation_year,degree,department,company,job_title,industry,location,linkedin_url,skills\n"
                      "John Doe,2022,MCA,Computer Apps,Google,Staff Eng,Tech,Bengaluru,https://linkedin.com/in/jd,\"C++,Go\"\n"
                      "=SUM(1+1),2020,BCA,Computer Apps,TCS,Analyst,IT,Pune,,\"Java\"\n";

    auto rows = CsvUtils::parseAlumniCsv(csv);
    TEST_ASSERT(rows.size() == 2, "Parsed 2 data rows");
    TEST_ASSERT(rows[0].fullName == "John Doe", "First row name parsed correctly");
    TEST_ASSERT(rows[0].skills.size() == 2 && rows[0].skills[0] == "C++", "Quoted comma-separated skills parsed");
    TEST_ASSERT(rows[1].fullName.front() == '\'', "Formula injection cell beginning with '=' was safely sanitized with leading quote");
}

int main() {
    std::cout << "========================================================\n";
    std::cout << "  AlumniVerse C++ Graph, Tree & Security Engine Tests  \n";
    std::cout << "========================================================\n";

    testEmptyGraph();
    testSingleVertex();
    testDuplicateAndInvalidEdges();
    testDisconnectedComponents();
    testCyclicGraphTraversalAndShortestPath();
    testTreeSubtreeExtractionAndPruning();
    testCryptoSecurity();
    testCsvParsingAndSanitization();

    std::cout << "\n========================================================\n";
    std::cout << "TEST RESULTS SUMMARY:\n";
    std::cout << "Passed: " << g_passedTests << "\n";
    std::cout << "Failed: " << g_failedTests << "\n";
    std::cout << "========================================================\n";

    return (g_failedTests == 0) ? 0 : 1;
}
