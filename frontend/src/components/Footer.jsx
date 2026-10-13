import React from 'react';
import { Cpu, Database, GitBranch, Shield, Sparkles } from 'lucide-react';

export const Footer = () => {
  return (
    <footer className="w-full border-t border-slate-800/80 bg-[#06080F] text-slate-400 py-10 px-4 sm:px-6 lg:px-8 mt-auto">
      <div className="max-w-7xl mx-auto grid grid-cols-1 md:grid-cols-4 gap-8 mb-8 text-sm">
        <div className="md:col-span-2">
          <div className="flex items-center gap-2 mb-3">
            <span className="font-extrabold text-lg text-white">AlumniVerse</span>
            <span className="text-[10px] uppercase tracking-wider px-2 py-0.5 rounded bg-cyan-950 text-cyan-400 border border-cyan-800/40 font-mono">
              v1.0.0 Stable
            </span>
          </div>
          <p className="text-slate-400 text-xs leading-relaxed max-w-md mb-4">
            High-performance alumni graph intelligence and hierarchical tree navigation platform.
            Engineered with a C++17 Crow microservices backend for authoritative BFS/DFS graph traversals,
            shortest-path network discovery, and real-time subtree filter pruning.
          </p>
          <div className="p-3 rounded-lg bg-amber-950/20 border border-amber-800/30 text-amber-300 text-xs flex items-start gap-2.5 max-w-md">
            <Sparkles className="w-4 h-4 text-amber-400 shrink-0 mt-0.5" />
            <span>
              <strong>Fictional Demo Environment:</strong> Configured with sample dataset for institutional modeling (IIPS DAVV).
              All employment details and profiles are purely fictional for simulation.
            </span>
          </div>
        </div>

        <div>
          <h4 className="text-xs font-semibold text-slate-200 uppercase tracking-wider mb-3">Core Stack</h4>
          <ul className="space-y-2 text-xs">
            <li className="flex items-center gap-2 text-slate-300">
              <Cpu className="w-3.5 h-3.5 text-cyan-400" /> C++17 Crow HTTP REST Engine
            </li>
            <li className="flex items-center gap-2 text-slate-300">
              <GitBranch className="w-3.5 h-3.5 text-purple-400" /> React Flow Interactive DAG Canvas
            </li>
            <li className="flex items-center gap-2 text-slate-300">
              <Database className="w-3.5 h-3.5 text-indigo-400" /> PostgreSQL Relational Storage
            </li>
            <li className="flex items-center gap-2 text-slate-300">
              <Shield className="w-3.5 h-3.5 text-emerald-400" /> Argon2 / SHA-256 Auth & Audit Logs
            </li>
          </ul>
        </div>

        <div>
          <h4 className="text-xs font-semibold text-slate-200 uppercase tracking-wider mb-3">Algorithms</h4>
          <ul className="space-y-2 text-xs">
            <li className="text-slate-300">Unweighted Shortest Path (BFS)</li>
            <li className="text-slate-300">Depth-First Network Walk (DFS)</li>
            <li className="text-slate-300">Subtree Pruning & Ancestral Retention</li>
            <li className="text-slate-300">Induced Subgraph Extraction</li>
          </ul>
        </div>
      </div>

      <div className="max-w-7xl mx-auto pt-6 border-t border-slate-800/60 flex flex-col sm:flex-row items-center justify-between gap-4 text-xs text-slate-500">
        <p>&copy; 2026 AlumniVerse Platform. All rights reserved.</p>
        <div className="flex items-center gap-4">
          <span className="flex items-center gap-1.5 text-emerald-400">
            <span className="w-2 h-2 rounded-full bg-emerald-500 animate-ping" />
            Backend API Active (C++ Port 8080)
          </span>
        </div>
      </div>
    </footer>
  );
};

export default Footer;
