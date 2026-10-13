import React, { useState } from 'react';
import { useNavigate } from 'react-router-dom';
import { LogIn, UserPlus, ShieldCheck, Sparkles, AlertCircle, Key, Mail, User } from 'lucide-react';
import { useAuth } from '../context/AuthContext';

export const LoginPage = () => {
  const navigate = useNavigate();
  const { login, register } = useAuth();

  const [isRegisterTab, setIsRegisterTab] = useState(false);
  const [name, setName] = useState('');
  const [email, setEmail] = useState('admin@alumniverse.edu');
  const [password, setPassword] = useState('Password123!');
  const [role, setRole] = useState('alumni');

  const [loading, setLoading] = useState(false);
  const [error, setError] = useState(null);

  const handleSubmit = async (e) => {
    e.preventDefault();
    setLoading(true);
    setError(null);
    try {
      if (isRegisterTab) {
        await register(name, email, password, role);
      } else {
        await login(email, password);
      }
      navigate('/explorer');
    } catch (err) {
      setError(err.message || 'Authentication error.');
    } finally {
      setLoading(false);
    }
  };

  const quickFill = (demoEmail, demoRole) => {
    setEmail(demoEmail);
    setPassword('Password123!');
    setIsRegisterTab(false);
  };

  return (
    <div className="min-h-[calc(100vh-4rem)] bg-[#080B12] flex items-center justify-center p-4">
      <div className="w-full max-w-md p-8 rounded-3xl glass-panel bg-[#0B101D]/90 border border-slate-800 shadow-2xl space-y-6">
        {/* Brand Header */}
        <div className="text-center">
          <div className="inline-flex p-3 rounded-2xl bg-gradient-to-tr from-cyan-500/20 via-indigo-500/20 to-purple-500/20 border border-cyan-500/30 text-cyan-400 mb-3 shadow-lg">
            <Sparkles className="w-6 h-6" />
          </div>
          <h2 className="text-xl font-extrabold text-white">AlumniVerse Access Gateway</h2>
          <p className="text-xs text-slate-400 mt-1">Authenticate into the graph network directory</p>
        </div>

        {/* Tab Switcher */}
        <div className="grid grid-cols-2 gap-1 p-1 rounded-xl bg-slate-900 border border-slate-800">
          <button
            type="button"
            onClick={() => { setIsRegisterTab(false); setError(null); }}
            className={`py-2 rounded-lg text-xs font-bold transition-all ${
              !isRegisterTab ? 'bg-cyan-500 text-slate-950 shadow-sm' : 'text-slate-400 hover:text-white'
            }`}
          >
            Sign In
          </button>
          <button
            type="button"
            onClick={() => { setIsRegisterTab(true); setError(null); }}
            className={`py-2 rounded-lg text-xs font-bold transition-all ${
              isRegisterTab ? 'bg-cyan-500 text-slate-950 shadow-sm' : 'text-slate-400 hover:text-white'
            }`}
          >
            New Registration
          </button>
        </div>

        {/* Error Alert */}
        {error && (
          <div className="p-3 rounded-xl bg-rose-950/40 border border-rose-800/50 text-rose-300 text-xs flex items-center gap-2">
            <AlertCircle className="w-4 h-4 shrink-0 text-rose-400" />
            <span>{error}</span>
          </div>
        )}

        {/* Form */}
        <form onSubmit={handleSubmit} className="space-y-4 text-xs">
          {isRegisterTab && (
            <div>
              <label className="text-[11px] font-semibold text-slate-400 block mb-1">Full Legal Name</label>
              <div className="relative">
                <User className="w-4 h-4 absolute left-3 top-2.5 text-slate-500" />
                <input
                  type="text"
                  required
                  placeholder="e.g. Aarav Sharma"
                  value={name}
                  onChange={(e) => setName(e.target.value)}
                  className="w-full pl-9 pr-3 py-2 rounded-xl bg-slate-900 border border-slate-700 text-white placeholder-slate-600 focus:outline-none focus:border-cyan-400"
                />
              </div>
            </div>
          )}

          <div>
            <label className="text-[11px] font-semibold text-slate-400 block mb-1">Email Address</label>
            <div className="relative">
              <Mail className="w-4 h-4 absolute left-3 top-2.5 text-slate-500" />
              <input
                type="email"
                required
                placeholder="name@alumniverse.edu"
                value={email}
                onChange={(e) => setEmail(e.target.value)}
                className="w-full pl-9 pr-3 py-2 rounded-xl bg-slate-900 border border-slate-700 text-white placeholder-slate-600 focus:outline-none focus:border-cyan-400"
              />
            </div>
          </div>

          <div>
            <label className="text-[11px] font-semibold text-slate-400 block mb-1">Password</label>
            <div className="relative">
              <Key className="w-4 h-4 absolute left-3 top-2.5 text-slate-500" />
              <input
                type="password"
                required
                placeholder="&bull;&bull;&bull;&bull;&bull;&bull;&bull;&bull;"
                value={password}
                onChange={(e) => setPassword(e.target.value)}
                className="w-full pl-9 pr-3 py-2 rounded-xl bg-slate-900 border border-slate-700 text-white placeholder-slate-600 focus:outline-none focus:border-cyan-400"
              />
            </div>
          </div>

          {isRegisterTab && (
            <div>
              <label className="text-[11px] font-semibold text-slate-400 block mb-1">Platform Role</label>
              <select
                value={role}
                onChange={(e) => setRole(e.target.value)}
                className="w-full px-3 py-2 rounded-xl bg-slate-900 border border-slate-700 text-white focus:outline-none focus:border-cyan-400"
              >
                <option value="alumni">Alumni Member</option>
                <option value="viewer">Guest / Public Viewer</option>
                <option value="admin">System Administrator</option>
              </select>
            </div>
          )}

          <button
            type="submit"
            disabled={loading}
            className="w-full py-2.5 rounded-xl bg-gradient-to-r from-cyan-500 via-blue-500 to-indigo-600 hover:from-cyan-400 text-slate-950 font-bold text-xs flex items-center justify-center gap-2 shadow-lg shadow-cyan-500/25 transition-all disabled:opacity-50"
          >
            {loading ? (
              <Sparkles className="w-4 h-4 animate-spin text-slate-950" />
            ) : isRegisterTab ? (
              <UserPlus className="w-4 h-4 text-slate-950" />
            ) : (
              <LogIn className="w-4 h-4 text-slate-950" />
            )}
            {isRegisterTab ? 'Create Authorized Account' : 'Authenticate & Enter'}
          </button>
        </form>

        {/* Quick Fill Test Accounts */}
        <div className="pt-4 border-t border-slate-800 text-center">
          <span className="text-[10px] uppercase font-bold tracking-wider text-slate-500 block mb-2">
            Instant Demo Account Sign-in
          </span>
          <div className="flex flex-wrap items-center justify-center gap-2">
            <button
              type="button"
              onClick={() => quickFill('admin@alumniverse.edu', 'admin')}
              className="px-2.5 py-1 rounded-lg bg-cyan-950/80 hover:bg-cyan-900/80 text-cyan-300 border border-cyan-800/50 text-[10px] font-semibold"
            >
              Admin Demo
            </button>
            <button
              type="button"
              onClick={() => quickFill('aarav.sharma@alumniverse.edu', 'alumni')}
              className="px-2.5 py-1 rounded-lg bg-indigo-950/80 hover:bg-indigo-900/80 text-indigo-300 border border-indigo-800/50 text-[10px] font-semibold"
            >
              Alumni Demo
            </button>
            <button
              type="button"
              onClick={() => quickFill('viewer@alumniverse.edu', 'viewer')}
              className="px-2.5 py-1 rounded-lg bg-slate-800 hover:bg-slate-700 text-slate-300 border border-slate-700 text-[10px] font-semibold"
            >
              Viewer Demo
            </button>
          </div>
          <p className="text-[10px] text-slate-500 mt-2 font-mono">Default Demo Password: Password123!</p>
        </div>
      </div>
    </div>
  );
};

export default LoginPage;
