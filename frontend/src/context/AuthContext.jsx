import React, { createContext, useContext, useState, useEffect } from 'react';
import apiClient from '../api/client';

const AuthContext = createContext(null);

export const AuthProvider = ({ children }) => {
  const [token, setToken] = useState(localStorage.getItem('alumniverse_token'));
  const [user, setUser] = useState(() => {
    try {
      const saved = localStorage.getItem('alumniverse_user');
      return saved ? JSON.parse(saved) : null;
    } catch {
      return null;
    }
  });
  const [loading, setLoading] = useState(false);

  useEffect(() => {
    const fetchUser = async () => {
      if (!token) return;
      try {
        const res = await apiClient.getMe();
        if (res.status === 'success') {
          setUser(res.data);
          localStorage.setItem('alumniverse_user', JSON.stringify(res.data));
        } else if (res.code === 401) {
          logout();
        }
      } catch (err) {
        console.warn('Silent token verification note:', err.message);
        // Do NOT log out on network error; maintain user session from localStorage
      }
    };
    fetchUser();
  }, [token]);

  const login = async (email, password) => {
    const res = await apiClient.login(email, password);
    if (res.status === 'success') {
      const { token: newToken, user: newUser } = res.data;
      localStorage.setItem('alumniverse_token', newToken);
      localStorage.setItem('alumniverse_user', JSON.stringify(newUser));
      setToken(newToken);
      setUser(newUser);
      return newUser;
    }
    throw new Error(res.message || 'Login failed.');
  };

  const register = async (name, email, password, role = 'alumni') => {
    const res = await apiClient.register(name, email, password, role);
    if (res.status === 'success') {
      const { token: newToken, user: newUser } = res.data;
      localStorage.setItem('alumniverse_token', newToken);
      localStorage.setItem('alumniverse_user', JSON.stringify(newUser));
      setToken(newToken);
      setUser(newUser);
      return newUser;
    }
    throw new Error(res.message || 'Registration failed.');
  };

  const logout = () => {
    localStorage.removeItem('alumniverse_token');
    localStorage.removeItem('alumniverse_user');
    setToken(null);
    setUser(null);
  };

  const isAdmin = user?.role === 'admin';
  const isAlumni = user?.role === 'alumni' || isAdmin;
  const isViewer = user?.role === 'viewer';
  const isAuthenticated = !!user;

  return (
    <AuthContext.Provider
      value={{
        user,
        token,
        loading,
        login,
        register,
        logout,
        isAdmin,
        isAlumni,
        isViewer,
        isAuthenticated,
      }}
    >
      {children}
    </AuthContext.Provider>
  );
};

export const useAuth = () => {
  const context = useContext(AuthContext);
  if (!context) {
    throw new Error('useAuth must be used within an AuthProvider');
  }
  return context;
};
