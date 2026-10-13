import React, { useState, useEffect } from 'react';
import {
  ShieldCheck,
  Upload,
  UserPlus,
  Link as LinkIcon,
  FileText,
  AlertCircle,
  CheckCircle2,
  Clock,
  Sparkles,
  Lock,
  ArrowRight,
  RefreshCw,
} from 'lucide-react';
import apiClient from '../api/client';
import { useAuth } from '../context/AuthContext';
import { Link } from 'react-router-dom';

export const AdminPage = () => {
  const { user, isAdmin, isAuthenticated } = useAuth();

  const [activeTab, setActiveTab] = useState('verification'); // 'verification' | 'csv' | 'create' | 'connections' | 'audit'
  const [alumniList, setAlumniList] = useState([]);
  const [auditLogs, setAuditLogs] = useState([]);
  const [loading, setLoading] = useState(false);
  const [actionSuccess, setActionSuccess] = useState(null);
  const [actionError, setActionError] = useState(null);

  // CSV Import State
  const [csvContent, setCsvContent] = useState('');
  const [csvPreviewRows, setCsvPreviewRows] = useState([]);
  const [csvResult, setCsvResult] = useState(null);

  // New Alumni Form State
  const [newAlumni, setNewAlumni] = useState({
    full_name: '',
    graduation_year: 2022,
    current_company: '',
    current_job_title: '',
    current_industry: 'Technology & Internet',
    current_location: 'Bengaluru, India',
    bio: '',
    linkedin_url: '',
    skills: 'C++, Distributed Systems',
  });

  // New Connection Form State
  const [newConnection, setNewConnection] = useState({
    source_alumni_id: '',
    target_alumni_id: '',
    relationship_type: 'colleague',
  });

  const loadData = async () => {
    if (!isAdmin) return;
    setLoading(true);
    try {
      const alRes = await apiClient.getAlumniList({ limit: 100 });
      if (alRes.status === 'success') {
        setAlumniList(alRes.data.items || []);
      }
      const logRes = await apiClient.getAuditLogs();
      if (logRes.status === 'success') {
        setAuditLogs(logRes.data || []);
      }
    } catch (err) {
      console.warn('Admin load error:', err.message);
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    loadData();
  }, [isAdmin]);

  const handleVerify = async (id, status = 'verified') => {
    try {
      setActionSuccess(null);
      setActionError(null);
      const res = await apiClient.verifyAlumni(id, status);
      if (res.status === 'success') {
        setActionSuccess(`Profile #${id} successfully marked as ${status}.`);
        loadData();
      }
    } catch (err) {
      setActionError(err.message || 'Verification update failed.');
    }
  };

  const handleCsvTextChange = (text) => {
    setCsvContent(text);
    const lines = text.trim().split('\n');
    if (lines.length > 1) {
      const rows = lines.slice(1, 6).map((l) => l.split(','));
      setCsvPreviewRows(rows);
    } else {
      setCsvPreviewRows([]);
    }
  };

  const handleCsvSubmit = async () => {
    if (!csvContent.trim()) {
      setActionError('Please provide CSV content to import.');
      return;
    }
    setLoading(true);
    setActionSuccess(null);
    setActionError(null);
    try {
      const res = await apiClient.importCsv(csvContent);
      if (res.status === 'success') {
        setCsvResult(res.data);
        setActionSuccess(`Import completed! Successfully imported ${res.data.imported_count} alumni profiles.`);
        loadData();
      }
    } catch (err) {
      setActionError(err.message || 'CSV batch import failed.');
    } finally {
      setLoading(false);
    }
  };

  const handleCreateAlumni = async (e) => {
    e.preventDefault();
    setLoading(true);
    setActionSuccess(null);
    setActionError(null);
    try {
      const payload = {
        ...newAlumni,
        graduation_year: Number(newAlumni.graduation_year),
        skills: newAlumni.skills.split(',').map((s) => s.trim()).filter(Boolean),
      };
      const res = await apiClient.createAlumni(payload);
      if (res.status === 'success') {
        setActionSuccess(`Alumni profile for ${res.data.full_name} created successfully!`);
        loadData();
        setNewAlumni({
          full_name: '',
          graduation_year: 2022,
          current_company: '',
          current_job_title: '',
          current_industry: 'Technology & Internet',
          current_location: 'Bengaluru, India',
          bio: '',
          linkedin_url: '',
          skills: 'C++, Distributed Systems',
        });
      }
    } catch (err) {
      setActionError(err.message || 'Failed to create alumni profile.');
    } finally {
      setLoading(false);
    }
  };

  const handleCreateConnection = async (e) => {
    e.preventDefault();
    if (!newConnection.source_alumni_id || !newConnection.target_alumni_id) {
      setActionError('Please select both source and target alumni.');
      return;
    }
    setLoading(true);
    setActionSuccess(null);
    setActionError(null);
    try {
      const res = await apiClient.addConnection(
        Number(newConnection.source_alumni_id),
        Number(newConnection.target_alumni_id),
        newConnection.relationship_type
      );
      if (res.status === 'success') {
        setActionSuccess('Professional connection edge confirmed and persisted to graph!');
        loadData();
      }
    } catch (err) {
      setActionError(err.message || 'Connection creation failed.');
    } finally {
      setLoading(false);
    }
  };

  if (!isAdmin) {
    return (
      <div className="min-h-screen bg-[#080B12] p-8 flex items-center justify-center">
        <div className="max-w-md w-full p-8 rounded-3xl glass-panel border border-slate-800 text-center space-y-4">
          <div className="w-14 h-14 rounded-2xl bg-amber-950/60 border border-amber-800/50 flex items-center justify-center mx-auto text-amber-400">
            <Lock className="w-6 h-6" />
          </div>
          <h2 className="text-lg font-bold text-white">Administrator Access Required</h2>
          <p className="text-xs text-slate-400 leading-relaxed">
            Administrative endpoints and profile verification are restricted to authenticated system administrators.
          </p>
          <div className="pt-2">
            <Link
              to="/login"
              className="inline-flex items-center gap-2 px-5 py-2.5 rounded-xl bg-gradient-to-r from-cyan-500 to-indigo-600 text-slate-950 font-bold text-xs"
            >
              Sign In with Admin Credentials <ArrowRight className="w-4 h-4" />
            </Link>
          </div>
        </div>
      </div>
    );
  }

  return (
    <div className="min-h-screen bg-[#080B12] text-slate-100 p-4 sm:p-6 lg:p-8 max-w-7xl mx-auto space-y-6">
      {/* Page Title */}
      <div className="flex flex-col sm:flex-row sm:items-center justify-between gap-4 pb-4 border-b border-slate-800">
        <div>
          <h1 className="text-2xl font-black text-white tracking-tight flex items-center gap-2.5">
            <ShieldCheck className="w-6 h-6 text-emerald-400" />
            Institutional Governance & Admin Hub
          </h1>
          <p className="text-slate-400 text-xs mt-1">
            Logged in as <strong>{user.name}</strong> ({user.role}) &bull; Manage verifications, batch CSV imports, and graph connections.
          </p>
        </div>

        <button
          onClick={loadData}
          className="self-start sm:self-auto px-3 py-1.5 rounded-xl bg-slate-900 border border-slate-700 hover:border-slate-500 text-slate-300 font-semibold text-xs flex items-center gap-1.5 transition-all"
        >
          <RefreshCw className={`w-3.5 h-3.5 ${loading ? 'animate-spin' : ''}`} />
          Refresh State
        </button>
      </div>

      {/* Action Notification Toasts */}
      {actionSuccess && (
        <div className="p-3.5 rounded-2xl bg-emerald-950/40 border border-emerald-800/50 text-emerald-300 text-xs flex items-center gap-2.5 shadow-lg">
          <CheckCircle2 className="w-4 h-4 text-emerald-400 shrink-0" />
          <span>{actionSuccess}</span>
        </div>
      )}
      {actionError && (
        <div className="p-3.5 rounded-2xl bg-rose-950/40 border border-rose-800/50 text-rose-300 text-xs flex items-center gap-2.5 shadow-lg">
          <AlertCircle className="w-4 h-4 text-rose-400 shrink-0" />
          <span>{actionError}</span>
        </div>
      )}

      {/* Admin Tabs */}
      <div className="flex flex-wrap gap-2 border-b border-slate-800 pb-3">
        {[
          { id: 'verification', label: 'Profile Verifications', icon: ShieldCheck },
          { id: 'csv', label: 'CSV Batch Import', icon: Upload },
          { id: 'create', label: 'Create Alumni', icon: UserPlus },
          { id: 'connections', label: 'Manage Connections', icon: LinkIcon },
          { id: 'audit', label: 'Audit Trail Logs', icon: FileText },
        ].map((tab) => {
          const Icon = tab.icon;
          const isActive = activeTab === tab.id;
          return (
            <button
              key={tab.id}
              onClick={() => setActiveTab(tab.id)}
              className={`px-4 py-2 rounded-xl text-xs font-bold flex items-center gap-2 transition-all ${
                isActive
                  ? 'bg-cyan-500 text-slate-950 shadow-md shadow-cyan-500/20'
                  : 'bg-slate-900/80 text-slate-400 hover:text-white border border-slate-800'
              }`}
            >
              <Icon className="w-4 h-4" />
              {tab.label}
            </button>
          );
        })}
      </div>

      {/* Tab 1: Verification Queue */}
      {activeTab === 'verification' && (
        <div className="space-y-4">
          <div className="p-4 rounded-2xl glass-panel border border-slate-800 flex items-center justify-between text-xs">
            <span className="text-slate-300">
              Total Profiles in Registry: <strong>{alumniList.length}</strong>
            </span>
            <span className="text-emerald-400 font-semibold">
              Verified: {alumniList.filter((a) => a.verification_status === 'verified').length}
            </span>
          </div>

          <div className="overflow-x-auto rounded-2xl glass-panel border border-slate-800">
            <table className="w-full text-left text-xs">
              <thead className="bg-slate-900/80 border-b border-slate-800 text-slate-400 uppercase font-semibold">
                <tr>
                  <th className="p-3.5">ID</th>
                  <th className="p-3.5">Name</th>
                  <th className="p-3.5">Company & Role</th>
                  <th className="p-3.5">Graduation</th>
                  <th className="p-3.5">Current Status</th>
                  <th className="p-3.5 text-right">Verification Action</th>
                </tr>
              </thead>
              <tbody className="divide-y divide-slate-800/60">
                {alumniList.map((al) => (
                  <tr key={al.id} className="hover:bg-slate-800/30">
                    <td className="p-3.5 font-mono text-slate-400">#{al.id}</td>
                    <td className="p-3.5 font-bold text-white">{al.full_name}</td>
                    <td className="p-3.5 text-cyan-400">{al.current_company} &bull; {al.current_job_title}</td>
                    <td className="p-3.5 text-slate-300">{al.degree} ({al.graduation_year})</td>
                    <td className="p-3.5">
                      <span
                        className={`px-2 py-0.5 rounded-full text-[10px] font-bold uppercase ${
                          al.verification_status === 'verified'
                            ? 'bg-emerald-950 text-emerald-300 border border-emerald-800/50'
                            : 'bg-amber-950 text-amber-300 border border-amber-800/50'
                        }`}
                      >
                        {al.verification_status}
                      </span>
                    </td>
                    <td className="p-3.5 text-right space-x-2">
                      {al.verification_status !== 'verified' ? (
                        <button
                          onClick={() => handleVerify(al.id, 'verified')}
                          className="px-2.5 py-1 rounded-lg bg-emerald-600 hover:bg-emerald-500 text-slate-950 font-bold text-[11px]"
                        >
                          Approve & Verify
                        </button>
                      ) : (
                        <button
                          onClick={() => handleVerify(al.id, 'pending')}
                          className="px-2.5 py-1 rounded-lg bg-slate-800 hover:bg-slate-700 text-slate-300 font-semibold text-[11px]"
                        >
                          Mark Pending
                        </button>
                      )}
                    </td>
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
        </div>
      )}

      {/* Tab 2: CSV Batch Import */}
      {activeTab === 'csv' && (
        <div className="p-6 rounded-2xl glass-panel border border-slate-800 space-y-5 text-xs">
          <div>
            <h3 className="text-sm font-bold text-white uppercase tracking-wider mb-1">
              Bulk CSV Alumni Import Engine
            </h3>
            <p className="text-slate-400 text-xs">
              Upload raw CSV data. Automatically parsed, validated, and protected against formula injection by C++ CsvUtils.
            </p>
          </div>

          <div>
            <label className="text-[11px] font-semibold text-slate-400 uppercase tracking-wider block mb-1">
              Paste CSV Content or Sample Data
            </label>
            <textarea
              rows={8}
              value={csvContent}
              onChange={(e) => handleCsvTextChange(e.target.value)}
              placeholder="full_name,graduation_year,degree,department,company,job_title,industry,location,linkedin_url,skills&#10;Tarun Joshi,2022,MCA,Computer Apps,Uber,Staff Backend Eng,Tech,Bengaluru,https://linkedin.com/in/tj,&quot;C++,Go&quot;"
              className="w-full p-3 rounded-xl bg-slate-950 border border-slate-700 font-mono text-xs text-white placeholder-slate-600 focus:outline-none focus:border-cyan-400"
            />
          </div>

          <div className="flex gap-3">
            <button
              onClick={() => {
                const sample = `full_name,graduation_year,degree,department,company,job_title,industry,location,linkedin_url,skills\nSiddharth Varma,2021,MCA (Integrated 5 Year),Department of Computer Applications,Uber,Staff Backend Engineer,Ride Hailing & Mobility,Bengaluru,https://linkedin.com/in/demo-siddharth-varma,"C++,Distributed Systems,Go"\nMeera Nambiar,2022,M.Tech (Information Technology),Department of Computer Applications,NVIDIA,Deep Learning Solutions Architect,Semiconductors & AI,Pune,https://linkedin.com/in/demo-meera-nambiar,"Machine Learning & Python,C++,CUDA"`;
                handleCsvTextChange(sample);
              }}
              className="px-3 py-1.5 rounded-lg bg-slate-800 hover:bg-slate-700 text-slate-300 font-semibold text-xs"
            >
              Fill Sample CSV Data
            </button>

            <button
              onClick={handleCsvSubmit}
              disabled={loading || !csvContent.trim()}
              className="px-5 py-2 rounded-xl bg-cyan-500 hover:bg-cyan-400 text-slate-950 font-bold text-xs flex items-center gap-1.5 shadow-md shadow-cyan-500/25 disabled:opacity-50"
            >
              <Upload className="w-4 h-4" />
              {loading ? 'Processing via C++...' : 'Validate & Import Batch'}
            </button>
          </div>

          {/* Preview */}
          {csvPreviewRows.length > 0 && (
            <div className="mt-4 pt-4 border-t border-slate-800">
              <h4 className="text-xs font-bold text-slate-300 uppercase mb-2">CSV Table Preview (First 5 Rows)</h4>
              <div className="overflow-x-auto rounded-xl border border-slate-800">
                <table className="w-full text-left text-[11px]">
                  <tbody className="divide-y divide-slate-800 bg-slate-900/60">
                    {csvPreviewRows.map((row, idx) => (
                      <tr key={idx}>
                        {row.map((cell, cIdx) => (
                          <td key={cIdx} className="p-2 border-r border-slate-800/80 font-mono text-slate-300">
                            {cell}
                          </td>
                        ))}
                      </tr>
                    ))}
                  </tbody>
                </table>
              </div>
            </div>
          )}
        </div>
      )}

      {/* Tab 3: Create Single Alumni */}
      {activeTab === 'create' && (
        <form onSubmit={handleCreateAlumni} className="p-6 rounded-2xl glass-panel border border-slate-800 space-y-4 text-xs">
          <h3 className="text-sm font-bold text-white uppercase tracking-wider mb-2">
            Register New Alumni Record
          </h3>

          <div className="grid grid-cols-1 sm:grid-cols-2 gap-4">
            <div>
              <label className="text-[11px] font-semibold text-slate-400 block mb-1">Full Name *</label>
              <input
                type="text"
                required
                value={newAlumni.full_name}
                onChange={(e) => setNewAlumni({ ...newAlumni, full_name: e.target.value })}
                className="w-full px-3 py-2 rounded-xl bg-slate-900 border border-slate-700 text-white"
                placeholder="e.g. Rahul Sharma"
              />
            </div>

            <div>
              <label className="text-[11px] font-semibold text-slate-400 block mb-1">Graduation Year *</label>
              <input
                type="number"
                required
                value={newAlumni.graduation_year}
                onChange={(e) => setNewAlumni({ ...newAlumni, graduation_year: e.target.value })}
                className="w-full px-3 py-2 rounded-xl bg-slate-900 border border-slate-700 text-white"
              />
            </div>

            <div>
              <label className="text-[11px] font-semibold text-slate-400 block mb-1">Current Company</label>
              <input
                type="text"
                value={newAlumni.current_company}
                onChange={(e) => setNewAlumni({ ...newAlumni, current_company: e.target.value })}
                className="w-full px-3 py-2 rounded-xl bg-slate-900 border border-slate-700 text-white"
                placeholder="e.g. Microsoft"
              />
            </div>

            <div>
              <label className="text-[11px] font-semibold text-slate-400 block mb-1">Current Job Title</label>
              <input
                type="text"
                value={newAlumni.current_job_title}
                onChange={(e) => setNewAlumni({ ...newAlumni, current_job_title: e.target.value })}
                className="w-full px-3 py-2 rounded-xl bg-slate-900 border border-slate-700 text-white"
                placeholder="e.g. Senior Software Engineer"
              />
            </div>

            <div>
              <label className="text-[11px] font-semibold text-slate-400 block mb-1">Location</label>
              <input
                type="text"
                value={newAlumni.current_location}
                onChange={(e) => setNewAlumni({ ...newAlumni, current_location: e.target.value })}
                className="w-full px-3 py-2 rounded-xl bg-slate-900 border border-slate-700 text-white"
              />
            </div>

            <div>
              <label className="text-[11px] font-semibold text-slate-400 block mb-1">Skills (Comma-separated)</label>
              <input
                type="text"
                value={newAlumni.skills}
                onChange={(e) => setNewAlumni({ ...newAlumni, skills: e.target.value })}
                className="w-full px-3 py-2 rounded-xl bg-slate-900 border border-slate-700 text-white"
              />
            </div>
          </div>

          <div>
            <label className="text-[11px] font-semibold text-slate-400 block mb-1">LinkedIn Profile URL</label>
            <input
              type="url"
              value={newAlumni.linkedin_url}
              onChange={(e) => setNewAlumni({ ...newAlumni, linkedin_url: e.target.value })}
              className="w-full px-3 py-2 rounded-xl bg-slate-900 border border-slate-700 text-white"
              placeholder="https://linkedin.com/in/username"
            />
          </div>

          <div>
            <label className="text-[11px] font-semibold text-slate-400 block mb-1">Biography / Career Summary</label>
            <textarea
              rows={3}
              value={newAlumni.bio}
              onChange={(e) => setNewAlumni({ ...newAlumni, bio: e.target.value })}
              className="w-full p-3 rounded-xl bg-slate-900 border border-slate-700 text-white"
            />
          </div>

          <button
            type="submit"
            disabled={loading}
            className="px-6 py-2.5 rounded-xl bg-cyan-500 hover:bg-cyan-400 text-slate-950 font-bold text-xs"
          >
            {loading ? 'Saving...' : 'Create Alumni Record'}
          </button>
        </form>
      )}

      {/* Tab 4: Connect Two Alumni */}
      {activeTab === 'connections' && (
        <form onSubmit={handleCreateConnection} className="p-6 rounded-2xl glass-panel border border-slate-800 space-y-4 text-xs">
          <h3 className="text-sm font-bold text-white uppercase tracking-wider mb-2">
            Establish Confirmed Relationship Edge
          </h3>
          <p className="text-slate-400 text-xs">
            Adds an authoritative undirected edge between two alumni in the C++ Graph adjacency list.
          </p>

          <div className="grid grid-cols-1 sm:grid-cols-3 gap-4">
            <div>
              <label className="text-[11px] font-semibold text-slate-400 block mb-1">Source Alumni *</label>
              <select
                required
                value={newConnection.source_alumni_id}
                onChange={(e) => setNewConnection({ ...newConnection, source_alumni_id: e.target.value })}
                className="w-full px-3 py-2 rounded-xl bg-slate-900 border border-slate-700 text-white"
              >
                <option value="">Select Source Alumni</option>
                {alumniList.map((a) => (
                  <option key={a.id} value={a.id}>
                    {a.full_name} (#{a.id})
                  </option>
                ))}
              </select>
            </div>

            <div>
              <label className="text-[11px] font-semibold text-slate-400 block mb-1">Target Alumni *</label>
              <select
                required
                value={newConnection.target_alumni_id}
                onChange={(e) => setNewConnection({ ...newConnection, target_alumni_id: e.target.value })}
                className="w-full px-3 py-2 rounded-xl bg-slate-900 border border-slate-700 text-white"
              >
                <option value="">Select Target Alumni</option>
                {alumniList.map((a) => (
                  <option key={a.id} value={a.id}>
                    {a.full_name} (#{a.id})
                  </option>
                ))}
              </select>
            </div>

            <div>
              <label className="text-[11px] font-semibold text-slate-400 block mb-1">Relationship Type *</label>
              <select
                value={newConnection.relationship_type}
                onChange={(e) => setNewConnection({ ...newConnection, relationship_type: e.target.value })}
                className="w-full px-3 py-2 rounded-xl bg-slate-900 border border-slate-700 text-white"
              >
                <option value="colleague">Colleague</option>
                <option value="classmate">Classmate</option>
                <option value="mentor">Mentor</option>
                <option value="professional_connection">Professional Connection</option>
              </select>
            </div>
          </div>

          <button
            type="submit"
            disabled={loading}
            className="px-6 py-2.5 rounded-xl bg-indigo-600 hover:bg-indigo-500 text-white font-bold text-xs"
          >
            {loading ? 'Adding Edge...' : 'Confirm Graph Connection'}
          </button>
        </form>
      )}

      {/* Tab 5: Audit Logs */}
      {activeTab === 'audit' && (
        <div className="space-y-4">
          <div className="overflow-x-auto rounded-2xl glass-panel border border-slate-800">
            <table className="w-full text-left text-xs">
              <thead className="bg-slate-900/80 border-b border-slate-800 text-slate-400 uppercase font-semibold">
                <tr>
                  <th className="p-3.5">Timestamp</th>
                  <th className="p-3.5">Actor</th>
                  <th className="p-3.5">Action</th>
                  <th className="p-3.5">Entity</th>
                  <th className="p-3.5">Audit Details</th>
                </tr>
              </thead>
              <tbody className="divide-y divide-slate-800/60 font-mono text-[11px]">
                {auditLogs.map((log) => (
                  <tr key={log.id} className="hover:bg-slate-800/30">
                    <td className="p-3.5 text-slate-500">{log.created_at}</td>
                    <td className="p-3.5 text-cyan-400 font-sans font-bold">{log.actor_name}</td>
                    <td className="p-3.5 text-purple-300">{log.action}</td>
                    <td className="p-3.5 text-slate-400">{log.entity_type} #{log.entity_id}</td>
                    <td className="p-3.5 text-slate-300 font-sans">{log.details}</td>
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
        </div>
      )}
    </div>
  );
};

export default AdminPage;
