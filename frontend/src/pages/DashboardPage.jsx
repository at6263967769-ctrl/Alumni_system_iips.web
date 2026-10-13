import React, { useEffect, useState } from 'react';
import { Link } from 'react-router-dom';
import {
  Users,
  ShieldCheck,
  Building2,
  Briefcase,
  TrendingUp,
  Clock,
  ArrowRight,
  ExternalLink,
  Sparkles,
  Network,
} from 'lucide-react';
import apiClient from '../api/client';

export const DashboardPage = () => {
  const [analytics, setAnalytics] = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  useEffect(() => {
    const fetchAnalytics = async () => {
      try {
        const res = await apiClient.getAnalytics();
        if (res.status === 'success') {
          setAnalytics(res.data);
        } else {
          setError(res.message || 'Failed to load analytics.');
        }
      } catch (err) {
        setError(err.message || 'Error communicating with backend.');
      } finally {
        setLoading(false);
      }
    };
    fetchAnalytics();
  }, []);

  if (loading) {
    return (
      <div className="min-h-screen bg-[#080B12] p-8 flex items-center justify-center">
        <div className="flex flex-col items-center gap-3 text-cyan-400">
          <Sparkles className="w-8 h-8 animate-spin" />
          <span className="text-xs font-mono uppercase tracking-wider">Loading Alumni Intelligence...</span>
        </div>
      </div>
    );
  }

  if (error || !analytics) {
    return (
      <div className="min-h-screen bg-[#080B12] p-8 flex items-center justify-center text-center">
        <div className="max-w-md p-6 rounded-2xl glass-panel bg-rose-950/20 border-rose-800/40 text-rose-300">
          <p className="font-bold text-sm mb-2">Error Loading Analytics</p>
          <p className="text-xs mb-4">{error}</p>
          <button
            onClick={() => window.location.reload()}
            className="px-4 py-2 rounded-lg bg-rose-900 text-white text-xs font-semibold"
          >
            Retry Connection
          </button>
        </div>
      </div>
    );
  }

  // Find max count for relative bar chart height scaling
  const maxYearCount = Math.max(
    1,
    ...(analytics.alumni_distribution_by_year?.map((y) => y.count) || [1])
  );

  return (
    <div className="min-h-screen bg-[#080B12] text-slate-100 p-4 sm:p-6 lg:p-8 max-w-7xl mx-auto space-y-8">
      {/* Page Header */}
      <div className="flex flex-col sm:flex-row sm:items-center justify-between gap-4 pb-4 border-b border-slate-800">
        <div>
          <h1 className="text-2xl font-black text-white tracking-tight flex items-center gap-2.5">
            <TrendingUp className="w-6 h-6 text-cyan-400" />
            Alumni Network Intelligence & Metrics
          </h1>
          <p className="text-slate-400 text-xs mt-1">
            Real-time analytics aggregating institution cohorts, industry distribution, and verification pipeline.
          </p>
        </div>

        <Link
          to="/explorer"
          className="self-start sm:self-auto px-4 py-2 rounded-xl bg-gradient-to-r from-cyan-500 to-indigo-600 hover:from-cyan-400 text-slate-950 font-bold text-xs flex items-center gap-2 shadow-lg shadow-cyan-500/25 transition-all"
        >
          <Network className="w-4 h-4 text-slate-950" />
          Open Visual Explorer
        </Link>
      </div>

      {/* KPI Cards Grid */}
      <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-4 gap-4">
        {/* Card 1 */}
        <div className="p-5 rounded-2xl glass-panel border border-slate-800 relative overflow-hidden group hover:border-cyan-500/50 transition-all">
          <div className="flex items-center justify-between mb-3">
            <span className="text-xs font-bold uppercase tracking-wider text-slate-400">Total Alumni</span>
            <div className="p-2 rounded-xl bg-cyan-950 text-cyan-400 border border-cyan-800/40">
              <Users className="w-4 h-4" />
            </div>
          </div>
          <div className="text-3xl font-extrabold text-white font-mono">{analytics.total_alumni}</div>
          <p className="text-[11px] text-slate-400 mt-1">Indexed in active database</p>
        </div>

        {/* Card 2 */}
        <div className="p-5 rounded-2xl glass-panel border border-slate-800 relative overflow-hidden group hover:border-emerald-500/50 transition-all">
          <div className="flex items-center justify-between mb-3">
            <span className="text-xs font-bold uppercase tracking-wider text-slate-400">Verified Profiles</span>
            <div className="p-2 rounded-xl bg-emerald-950 text-emerald-400 border border-emerald-800/40">
              <ShieldCheck className="w-4 h-4" />
            </div>
          </div>
          <div className="text-3xl font-extrabold text-emerald-400 font-mono">{analytics.verified_profiles}</div>
          <p className="text-[11px] text-slate-400 mt-1">
            {Math.round((analytics.verified_profiles / Math.max(1, analytics.total_alumni)) * 100)}% verified credentials
          </p>
        </div>

        {/* Card 3 */}
        <div className="p-5 rounded-2xl glass-panel border border-slate-800 relative overflow-hidden group hover:border-purple-500/50 transition-all">
          <div className="flex items-center justify-between mb-3">
            <span className="text-xs font-bold uppercase tracking-wider text-slate-400">Companies Represented</span>
            <div className="p-2 rounded-xl bg-purple-950 text-purple-400 border border-purple-800/40">
              <Building2 className="w-4 h-4" />
            </div>
          </div>
          <div className="text-3xl font-extrabold text-purple-300 font-mono">{analytics.companies_represented}</div>
          <p className="text-[11px] text-slate-400 mt-1">Across global tech & finance</p>
        </div>

        {/* Card 4 */}
        <div className="p-5 rounded-2xl glass-panel border border-slate-800 relative overflow-hidden group hover:border-indigo-500/50 transition-all">
          <div className="flex items-center justify-between mb-3">
            <span className="text-xs font-bold uppercase tracking-wider text-slate-400">Industries Covered</span>
            <div className="p-2 rounded-xl bg-indigo-950 text-indigo-400 border border-indigo-800/40">
              <Briefcase className="w-4 h-4" />
            </div>
          </div>
          <div className="text-3xl font-extrabold text-indigo-300 font-mono">{analytics.industries_represented}</div>
          <p className="text-[11px] text-slate-400 mt-1">Domain sectors identified</p>
        </div>
      </div>

      {/* Graduation Year Distribution Chart */}
      <div className="p-6 rounded-2xl glass-panel border border-slate-800">
        <div className="flex items-center justify-between mb-6">
          <div>
            <h3 className="text-sm font-bold text-white uppercase tracking-wider">
              Alumni Distribution by Graduation Year
            </h3>
            <p className="text-[11px] text-slate-400">Cohort volume over graduating classes</p>
          </div>
          <span className="text-[10px] font-mono px-2 py-0.5 rounded bg-slate-900 text-cyan-400 border border-slate-700">
            Database Aggregated
          </span>
        </div>

        {/* Visual Bar Chart */}
        <div className="grid grid-cols-2 sm:grid-cols-4 md:grid-cols-6 gap-3 items-end h-56 pt-8 pb-2">
          {analytics.alumni_distribution_by_year?.map((item) => {
            const heightPercent = Math.max(15, Math.round((item.count / maxYearCount) * 100));
            return (
              <div key={item.year} className="flex flex-col items-center gap-2 h-full justify-end group">
                <span className="text-xs font-mono font-bold text-cyan-400 group-hover:scale-125 transition-transform">
                  {item.count}
                </span>
                <div className="w-full max-w-[50px] bg-slate-800/80 rounded-t-xl overflow-hidden p-0.5 border-t border-cyan-500/50 flex flex-col justify-end">
                  <div
                    style={{ height: `${heightPercent}%` }}
                    className="w-full rounded-t-lg bg-gradient-to-t from-indigo-600 via-cyan-500 to-cyan-300 shadow-lg shadow-cyan-500/20 group-hover:brightness-125 transition-all duration-300"
                  />
                </div>
                <span className="text-[11px] font-mono text-slate-400 font-medium">Class of '{String(item.year).slice(-2)}</span>
              </div>
            );
          })}
        </div>
      </div>

      {/* Recent Verified Profile Updates */}
      <div className="p-6 rounded-2xl glass-panel border border-slate-800">
        <div className="flex items-center justify-between mb-4">
          <div className="flex items-center gap-2">
            <Clock className="w-4 h-4 text-emerald-400" />
            <h3 className="text-sm font-bold text-white uppercase tracking-wider">
              Recent Verified Profile Credentials
            </h3>
          </div>
          <Link to="/directory" className="text-xs text-cyan-400 hover:underline flex items-center gap-1">
            Browse All Profiles <ArrowRight className="w-3.5 h-3.5" />
          </Link>
        </div>

        <div className="divide-y divide-slate-800/80">
          {analytics.recent_verified_updates?.map((al) => (
            <div key={al.id} className="py-3.5 flex items-center justify-between gap-4">
              <div className="flex items-center gap-3">
                <div className="w-9 h-9 rounded-xl bg-gradient-to-tr from-cyan-600 to-indigo-600 font-bold text-xs flex items-center justify-center text-white">
                  {al.name.slice(0, 2).toUpperCase()}
                </div>
                <div>
                  <h4 className="text-xs font-bold text-white">{al.name}</h4>
                  <p className="text-[11px] text-cyan-400">{al.company} &bull; {al.job_title}</p>
                </div>
              </div>

              <div className="text-right">
                <span className="inline-flex items-center gap-1 px-2 py-0.5 rounded-full text-[10px] bg-emerald-950 text-emerald-300 border border-emerald-800/50 font-semibold mb-1">
                  <ShieldCheck className="w-3 h-3 text-emerald-400" /> Verified
                </span>
                <span className="text-[10px] text-slate-500 block font-mono">{al.verified_at}</span>
              </div>
            </div>
          ))}
        </div>
      </div>
    </div>
  );
};

export default DashboardPage;
