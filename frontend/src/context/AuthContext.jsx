import React, { createContext, useContext, useState, useEffect } from 'react';
import apiClient from '../api/client';

const AuthContext = createContext(null);

export const AuthProvider = ({ children }) => {
  const [user, setUser] = useState(null);
  const [token, setToken] = useState(localStorage.getItem('alumniverse_token'));
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    const fetchUser = async () => {
      if (!token) {
        setLoading(false);
        return;
      }
      try {
        const res = await apiClient.getMe();
        if (res.status === 'success') {
          setUser(res.data);
        } else {
          logout();
        }
      } catch (err) {
        console.warn('Token validation failed:', err.message);
        logout();
      } finally {
        setLoading(false);
      }
    };
    fetchUser();
  }, [token]);

  const login = async (email, password) => {
    const res = await apiClient.login(email, password);
    if (res.status === 'success') {
      const { token: newToken, user: newUser } = res.data;
      localStorage.setItem('alumniverse_token', newToken);
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
      setToken(newToken);
      setUser(newUser);
      return newUser;
    }
    throw new Error(res.message || 'Registration failed.');
  };

  const logout = () => {
    localStorage.removeItem('alumniverse_token');
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
