import React, { memo } from 'react';
import { Handle, Position } from '@xyflow/react';
import { ShieldCheck, Clock, Building2, GraduationCap, Briefcase, Award } from 'lucide-react';

export const AlumniNode = memo(({ data, selected }) => {
  const al = data.alumni || data;
  const isVerified = (al.verification_status || al.verificationStatus) === 'verified';
  const name = al.full_name || al.name || 'Alumni Member';
  const company = al.current_company || al.company || 'Independent';
  const role = al.current_job_title || al.job_title || 'Professional';
  const gradYear = al.graduation_year || '';
  const degree = al.degree || '';
  const skills = al.skills || [];
  const avatar = al.avatar_url;

  const initials = name
    .split(' ')
    .map((n) => n[0])
    .join('')
    .slice(0, 2)
    .toUpperCase();

  return (
    <div
      className={`relative w-64 rounded-xl p-3.5 transition-all duration-200 cursor-pointer text-left ${
        selected
          ? 'bg-[#15233D] border-2 border-cyan-400 shadow-xl shadow-cyan-500/25 scale-[1.03]'
          : 'bg-[#101726]/90 hover:bg-[#131D31] border border-slate-700/80 hover:border-cyan-500/50 shadow-lg'
      }`}
    >
      <Handle type="target" position={Position.Top} className="!bg-cyan-400 !w-2.5 !h-2.5" />

      {/* Top Header */}
      <div className="flex items-center gap-3 mb-2.5">
        <div className="relative shrink-0">
          {avatar ? (
            <img
              src={avatar}
              alt={name}
              className="w-10 h-10 rounded-full object-cover border border-cyan-500/40 shadow-sm"
              onError={(e) => {
                e.target.style.display = 'none';
                e.target.nextSibling.style.display = 'flex';
              }}
            />
          ) : null}
          <div
            className={`w-10 h-10 rounded-full bg-gradient-to-tr from-cyan-600 to-indigo-600 text-white font-bold text-xs items-center justify-center border border-cyan-400/50 ${
              avatar ? 'hidden' : 'flex'
            }`}
          >
            {initials}
          </div>
          {isVerified ? (
            <span
              title="Verified Profile"
              className="absolute -bottom-1 -right-1 bg-emerald-500 text-slate-950 p-0.5 rounded-full ring-2 ring-[#080B12]"
            >
              <ShieldCheck className="w-3 h-3 text-white" />
            </span>
          ) : (
            <span
              title="Pending Verification"
              className="absolute -bottom-1 -right-1 bg-amber-500 text-slate-950 p-0.5 rounded-full ring-2 ring-[#080B12]"
            >
              <Clock className="w-3 h-3 text-white" />
            </span>
          )}
        </div>

        <div className="min-w-0 flex-1">
          <h4 className="text-xs font-bold text-slate-100 truncate group-hover:text-cyan-300">
            {name}
          </h4>
          <p className="text-[11px] font-medium text-cyan-400 truncate flex items-center gap-1">
            <Building2 className="w-3 h-3 shrink-0" />
            {company}
          </p>
          <p className="text-[10px] text-slate-400 truncate">{role}</p>
        </div>
      </div>

      {/* Degree & Year */}
      {(degree || gradYear) && (
        <div className="flex items-center gap-1.5 text-[10px] text-slate-400 mb-2 py-1 px-2 rounded bg-slate-900/60 border border-slate-800/80">
          <GraduationCap className="w-3 h-3 text-purple-400 shrink-0" />
          <span className="truncate">
            {degree} {gradYear ? `'${String(gradYear).slice(-2)}` : ''}
          </span>
        </div>
      )}

      {/* Skill Tags */}
      {skills.length > 0 && (
        <div className="flex flex-wrap gap-1 mt-1">
          {skills.slice(0, 2).map((sk, idx) => (
            <span
              key={idx}
              className="text-[9px] px-1.5 py-0.5 rounded bg-slate-800/90 text-slate-300 border border-slate-700/50 font-mono truncate max-w-[100px]"
            >
              {sk}
            </span>
          ))}
          {skills.length > 2 && (
            <span className="text-[9px] px-1 py-0.5 text-slate-400">+{skills.length - 2}</span>
          )}
        </div>
      )}

      <Handle type="source" position={Position.Bottom} className="!bg-cyan-400 !w-2.5 !h-2.5" />
    </div>
  );
});

export const HierarchyNode = memo(({ data, selected }) => {
  const label = data.label || 'Node';
  const type = data.type || 'department';
  const count = data.alumniCount ?? 0;

  let icon = <Building2 className="w-4 h-4 text-purple-400" />;
  let badgeColor = 'bg-purple-950/80 text-purple-300 border-purple-800/50';

  if (type === 'institution') {
    icon = <GraduationCap className="w-4 h-4 text-cyan-400" />;
    badgeColor = 'bg-cyan-950/80 text-cyan-300 border-cyan-800/50';
  } else if (type === 'batch') {
    icon = <Briefcase className="w-4 h-4 text-indigo-400" />;
    badgeColor = 'bg-indigo-950/80 text-indigo-300 border-indigo-800/50';
  } else if (type === 'company' || type === 'industry') {
    icon = <Award className="w-4 h-4 text-emerald-400" />;
    badgeColor = 'bg-emerald-950/80 text-emerald-300 border-emerald-800/50';
  }

  return (
    <div
      className={`min-w-48 max-w-64 rounded-xl p-3 text-center transition-all duration-200 cursor-pointer ${
        selected
          ? 'bg-purple-950/60 border-2 border-purple-400 shadow-xl shadow-purple-500/25 scale-105'
          : 'bg-[#111827]/90 hover:bg-[#182234] border border-purple-500/30 hover:border-purple-400 shadow-md'
      }`}
    >
      <Handle type="target" position={Position.Top} className="!bg-purple-400 !w-2.5 !h-2.5" />

      <div className="flex items-center justify-center gap-2 mb-1.5">
        <div className="p-1.5 rounded-lg bg-slate-900/80 border border-slate-700/60">{icon}</div>
        <span className="text-[10px] uppercase font-bold tracking-wider text-slate-400">
          {type}
        </span>
      </div>

      <h4 className="text-xs font-bold text-slate-100 mb-2 truncate px-1" title={label}>
        {label}
      </h4>

      <div className="inline-flex items-center gap-1 px-2.5 py-0.5 rounded-full text-[10px] font-semibold border font-mono">
        <span className={`px-2 py-0.5 rounded-full border ${badgeColor}`}>
          {count} {count === 1 ? 'alumni' : 'alumni'}
        </span>
      </div>

      <Handle type="source" position={Position.Bottom} className="!bg-purple-400 !w-2.5 !h-2.5" />
    </div>
  );
});

export const nodeTypes = {
  alumniNode: AlumniNode,
  hierarchyNode: HierarchyNode,
};
