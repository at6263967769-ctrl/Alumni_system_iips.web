import React, { useEffect, useState } from 'react';
import { Link } from 'react-router-dom';
import {
  Network,
  GitBranch,
  ShieldCheck,
  Search,
  Route,
  ArrowRight,
  Database,
  Building2,
  Users,
  Briefcase,
  Layers,
  Sparkles,
} from 'lucide-react';
import apiClient from '../api/client';

export const LandingPage = () => {
  const [stats, setStats] = useState({
    total_alumni: 12,
    verified_profiles: 11,
    companies_represented: 10,
    industries_represented: 6,
    connections_count: 9,
  });

  useEffect(() => {
    const fetchAnalytics = async () => {
      try {
        const res = await apiClient.getAnalytics();
        if (res.status === 'success') {
          setStats(res.data);
        }
      } catch (err) {
        console.warn('Analytics fetch error:', err.message);
      }
    };
    fetchAnalytics();
  }, []);

  return (
    <div className="min-h-screen bg-[#080B12] text-slate-100 flex flex-col">
      {/* Hero Section */}
      <section className="relative overflow-hidden pt-20 pb-24 px-4 sm:px-6 lg:px-8 border-b border-slate-800/80">
        {/* Ambient background glows */}
        <div className="absolute top-1/4 left-1/2 -translate-x-1/2 -translate-y-1/2 w-[650px] h-[350px] bg-gradient-to-tr from-cyan-600/20 via-purple-600/20 to-indigo-600/10 blur-[130px] pointer-events-none rounded-full" />

        <div className="max-w-5xl mx-auto text-center relative z-10">
          <div className="inline-flex items-center gap-2 px-3 py-1.5 rounded-full bg-slate-900/90 border border-cyan-500/30 text-cyan-400 text-xs font-semibold mb-6 shadow-lg shadow-cyan-500/10">
            <Sparkles className="w-4 h-4 text-cyan-400 animate-spin" style={{ animationDuration: '6s' }} />
            <span>Interactive Alumni Tree & Graph Intelligence Platform</span>
            <span className="w-1.5 h-1.5 rounded-full bg-cyan-400" />
            <span className="text-slate-400 font-mono text-[11px]">C++ Crow Engine</span>
          </div>

          <h1 className="text-4xl sm:text-6xl font-black tracking-tight text-white mb-6 leading-tight">
            Map, Explore & Visualize Your <br />
            <span className="bg-gradient-to-r from-cyan-400 via-indigo-300 to-purple-400 bg-clip-text text-transparent">
              Alumni Universe with C++ Speed
            </span>
          </h1>

          <p className="text-slate-300 text-base sm:text-lg max-w-3xl mx-auto mb-10 leading-relaxed">
            Move beyond static directory lists. Explore high-performance hierarchical academic trees,
            walk verified professional relationships with BFS shortest path algorithms,
            and filter descendants seamlessly in real time.
          </p>

          {/* Call to Actions */}
          <div className="flex flex-col sm:flex-row items-center justify-center gap-4 mb-16">
            <Link
              to="/explorer"
              className="w-full sm:w-auto px-7 py-3.5 rounded-xl bg-gradient-to-r from-cyan-500 via-blue-500 to-indigo-600 hover:from-cyan-400 hover:to-indigo-500 text-slate-950 font-extrabold text-sm tracking-wide shadow-xl shadow-cyan-500/30 hover:shadow-cyan-500/50 transition-all flex items-center justify-center gap-2.5"
            >
              <Network className="w-5 h-5 text-slate-950" />
              Launch Interactive Explorer
              <ArrowRight className="w-4 h-4 text-slate-950" />
            </Link>

            <Link
              to="/directory"
              className="w-full sm:w-auto px-7 py-3.5 rounded-xl bg-slate-900/90 hover:bg-slate-800 text-slate-200 font-bold text-sm border border-slate-700/80 hover:border-slate-500 transition-all flex items-center justify-center gap-2"
            >
              <Search className="w-4 h-4 text-slate-400" />
              Search Alumni Directory
            </Link>
          </div>

          {/* Real-time backend metrics counters */}
          <div className="grid grid-cols-2 md:grid-cols-4 gap-4 p-5 rounded-2xl glass-panel bg-slate-900/40 border border-slate-800/80 shadow-2xl">
            <div className="p-3 text-center">
              <span className="text-2xl sm:text-3xl font-extrabold text-white block font-mono">
                {stats.total_alumni}
              </span>
              <span className="text-[11px] text-slate-400 uppercase tracking-wider font-semibold">
                Alumni Profiles
              </span>
            </div>
            <div className="p-3 text-center border-l border-slate-800/80">
              <span className="text-2xl sm:text-3xl font-extrabold text-emerald-400 block font-mono">
                {stats.verified_profiles}
              </span>
              <span className="text-[11px] text-slate-400 uppercase tracking-wider font-semibold">
                Verified Records
              </span>
            </div>
            <div className="p-3 text-center border-l border-slate-800/80">
              <span className="text-2xl sm:text-3xl font-extrabold text-cyan-400 block font-mono">
                {stats.companies_represented}
              </span>
              <span className="text-[11px] text-slate-400 uppercase tracking-wider font-semibold">
                Companies Represented
              </span>
            </div>
            <div className="p-3 text-center border-l border-slate-800/80">
              <span className="text-2xl sm:text-3xl font-extrabold text-purple-400 block font-mono">
                {stats.connections_count}
              </span>
              <span className="text-[11px] text-slate-400 uppercase tracking-wider font-semibold">
                Confirmed Edges
              </span>
            </div>
          </div>
        </div>
      </section>

      {/* Feature Architecture Cards */}
      <section className="py-20 px-4 sm:px-6 lg:px-8 max-w-7xl mx-auto w-full">
        <div className="text-center max-w-3xl mx-auto mb-16">
          <h2 className="text-2xl sm:text-3xl font-extrabold text-white mb-3">
            Architected for Institutional Intelligence
          </h2>
          <p className="text-slate-400 text-sm">
            Powered by a decoupled C++ REST backend for low latency and mathematical rigor.
          </p>
        </div>

        <div className="grid grid-cols-1 md:grid-cols-3 gap-6">
          {/* Card 1 */}
          <div className="p-6 rounded-2xl glass-card hover:border-cyan-500/40 transition-all duration-300 group">
            <div className="w-12 h-12 rounded-xl bg-cyan-950/80 border border-cyan-800/50 flex items-center justify-center text-cyan-400 mb-5 group-hover:scale-110 transition-transform">
              <GitBranch className="w-6 h-6" />
            </div>
            <h3 className="text-base font-bold text-white mb-2">Hierarchical Tree Navigation</h3>
            <p className="text-slate-400 text-xs leading-relaxed">
              Dynamically generates institutional trees (Institution &rarr; Department &rarr; Degree &rarr; Batch &rarr; Alumni)
              or industry trees. Prunes unmatched branches while preserving ancestral lineages.
            </p>
          </div>

          {/* Card 2 */}
          <div className="p-6 rounded-2xl glass-card hover:border-purple-500/40 transition-all duration-300 group">
            <div className="w-12 h-12 rounded-xl bg-purple-950/80 border border-purple-800/50 flex items-center justify-center text-purple-400 mb-5 group-hover:scale-110 transition-transform">
              <Route className="w-6 h-6" />
            </div>
            <h3 className="text-base font-bold text-white mb-2">BFS Shortest Path Finder</h3>
            <p className="text-slate-400 text-xs leading-relaxed">
              Authoritative C++ Graph engine calculates the exact degrees of separation between any two alumni.
              Detects mentor, colleague, and classmate relationships across batches.
            </p>
          </div>

          {/* Card 3 */}
          <div className="p-6 rounded-2xl glass-card hover:border-emerald-500/40 transition-all duration-300 group">
            <div className="w-12 h-12 rounded-xl bg-emerald-950/80 border border-emerald-800/50 flex items-center justify-center text-emerald-400 mb-5 group-hover:scale-110 transition-transform">
              <ShieldCheck className="w-6 h-6" />
            </div>
            <h3 className="text-base font-bold text-white mb-2">Verified Profile Governance</h3>
            <p className="text-slate-400 text-xs leading-relaxed">
              Administrative verification pipeline, CSV batch import with injection sanitization,
              single-current-job constraints, and persistent audit logs.
            </p>
          </div>
        </div>
      </section>

      {/* Institutional Modeling Banner */}
      <section className="py-12 px-4 sm:px-6 lg:px-8 max-w-7xl mx-auto w-full mb-12">
        <div className="p-8 rounded-3xl glass-panel bg-gradient-to-r from-purple-950/40 via-slate-900 to-cyan-950/40 border border-slate-800 flex flex-col md:flex-row items-center justify-between gap-6">
          <div>
            <span className="text-[10px] uppercase font-bold tracking-wider text-cyan-400 block mb-1">
              DAVV IIPS & Multi-Institution Ready
            </span>
            <h3 className="text-xl font-bold text-white mb-2">
              Configurable for Any Academic Department or University
            </h3>
            <p className="text-slate-400 text-xs max-w-xl">
              Currently pre-loaded with sample batches from the International Institute of Professional Studies
              (IIPS DAVV) including MCA, M.Tech, BCA, MBA, and B.Com graduates across top technology & finance firms.
            </p>
          </div>
          <Link
            to="/explorer"
            className="px-6 py-3 rounded-xl bg-white text-slate-950 font-bold text-xs hover:bg-slate-200 transition-all shrink-0 flex items-center gap-2"
          >
            Explore IIPS DAVV Tree <ArrowRight className="w-4 h-4" />
          </Link>
        </div>
      </section>
    </div>
  );
};

export default LandingPage;
