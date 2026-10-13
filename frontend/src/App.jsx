import React from 'react';
import { BrowserRouter as Router, Routes, Route, useLocation } from 'react-router-dom';
import { AuthProvider } from './context/AuthContext';
import Navbar from './components/Navbar';
import Footer from './components/Footer';

import LandingPage from './pages/LandingPage';
import DashboardPage from './pages/DashboardPage';
import ExplorerPage from './pages/ExplorerPage';
import DirectoryPage from './pages/DirectoryPage';
import ProfileDetailPage from './pages/ProfileDetailPage';
import AdminPage from './pages/AdminPage';
import LoginPage from './pages/LoginPage';

const AppLayout = () => {
  const location = useLocation();
  const isExplorer = location.pathname === '/explorer';

  return (
    <div className="min-h-screen flex flex-col bg-[#080B12] text-slate-100">
      <Navbar />
      <div className="flex-1">
        <Routes>
          <Route path="/" element={<LandingPage />} />
          <Route path="/dashboard" element={<DashboardPage />} />
          <Route path="/explorer" element={<ExplorerPage />} />
          <Route path="/directory" element={<DirectoryPage />} />
          <Route path="/alumni/:id" element={<ProfileDetailPage />} />
          <Route path="/admin" element={<AdminPage />} />
          <Route path="/login" element={<LoginPage />} />
          <Route path="*" element={<LandingPage />} />
        </Routes>
      </div>
      {!isExplorer && <Footer />}
    </div>
  );
};

export const App = () => {
  return (
    <AuthProvider>
      <Router>
        <AppLayout />
      </Router>
    </AuthProvider>
  );
};

export default App;
