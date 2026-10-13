import React, { useState } from 'react';
import {
  Search,
  Filter,
  RotateCcw,
  Layers,
  Building,
  GraduationCap,
  Briefcase,
  ShieldCheck,
  ChevronDown,
  X,
  Compass,
  GitBranch,
} from 'lucide-react';

export const FilterSidebar = ({
  viewMode,
  setViewMode,
  hierarchyType,
  setHierarchyType,
  filters,
  setFilters,
  onResetFilters,
  metadata,
  nodeCount,
  isLoading,
  selectedNode,
  onClearSelectedNode,
}) => {
  const [collapsed, setCollapsed] = useState(false);

  const handleInputChange = (field, value) => {
    setFilters((prev) => ({
      ...prev,
      [field]: value,
    }));
  };

  const handleSkillToggle = (skillName) => {
    setFilters((prev) => {
      const skills = prev.skills || [];
      const exists = skills.includes(skillName);
      const newSkills = exists ? skills.filter((s) => s !== skillName) : [...skills, skillName];
      return { ...prev, skills: newSkills };
    });
  };

  const activeChips = [];
  if (filters.search) activeChips.push({ key: 'search', label: `Search: "${filters.search}"` });
  if (filters.batch_id) {
    const b = metadata?.batches?.find((item) => String(item.id) === String(filters.batch_id));
    activeChips.push({ key: 'batch_id', label: b ? `${b.degree} (${b.graduation_year})` : `Batch: ${filters.batch_id}` });
  }
  if (filters.company) activeChips.push({ key: 'company', label: `Company: ${filters.company}` });
  if (filters.industry) activeChips.push({ key: 'industry', label: `Industry: ${filters.industry}` });
  if (filters.verification_status) activeChips.push({ key: 'verification_status', label: `Status: ${filters.verification_status}` });
  if (filters.skills?.length > 0) {
    filters.skills.forEach((sk) => {
      activeChips.push({ key: `skill-${sk}`, skillName: sk, label: `Skill: ${sk}` });
    });
  }

  const removeChip = (chip) => {
    if (chip.skillName) {
      handleSkillToggle(chip.skillName);
    } else {
      handleInputChange(chip.key, '');
    }
  };

  const availableSkills = [
    'C++',
    'Distributed Systems',
    'React & JavaScript',
    'System Design',
    'Cloud Architecture (AWS/GCP)',
    'PostgreSQL',
    'Kubernetes & Docker',
    'Product Management',
    'Financial Modeling',
  ];

  return (
    <aside
      className={`glass-panel border-r border-slate-800/80 transition-all duration-300 flex flex-col z-20 ${
        collapsed ? 'w-14' : 'w-80'
      }`}
    >
      {/* Sidebar Header */}
      <div className="p-3.5 border-b border-slate-800/80 flex items-center justify-between bg-slate-900/60">
        {!collapsed && (
          <div className="flex items-center gap-2">
            <Filter className="w-4 h-4 text-cyan-400" />
            <h3 className="text-xs font-bold uppercase tracking-wider text-slate-200">Graph Filters</h3>
            <span className="text-[10px] font-mono px-1.5 py-0.5 rounded bg-cyan-950 text-cyan-400 border border-cyan-800/40">
              {nodeCount} nodes
            </span>
          </div>
        )}
        <button
          onClick={() => setCollapsed(!collapsed)}
          className="p-1 rounded-lg text-slate-400 hover:text-white hover:bg-slate-800/60 transition-colors ml-auto"
          title={collapsed ? 'Expand Filters' : 'Collapse Filters'}
        >
          <ChevronDown className={`w-4 h-4 transition-transform duration-300 ${collapsed ? '-rotate-90' : 'rotate-90'}`} />
        </button>
      </div>

      {!collapsed && (
        <div className="flex-1 overflow-y-auto p-4 space-y-5 text-xs">
          {/* View Mode Switcher */}
          <div>
            <label className="text-[11px] font-semibold text-slate-400 uppercase tracking-wider block mb-1.5">
              Visualization Mode
            </label>
            <div className="grid grid-cols-2 gap-1 p-1 rounded-xl bg-slate-900/80 border border-slate-800">
              <button
                type="button"
                onClick={() => setViewMode('tree')}
                className={`py-1.5 px-2 rounded-lg text-xs font-semibold flex items-center justify-center gap-1.5 transition-all ${
                  viewMode === 'tree'
                    ? 'bg-gradient-to-r from-purple-600 to-indigo-600 text-white shadow-sm'
                    : 'text-slate-400 hover:text-slate-200'
                }`}
              >
                <GitBranch className="w-3.5 h-3.5" />
                Hierarchy Tree
              </button>
              <button
                type="button"
                onClick={() => setViewMode('graph')}
                className={`py-1.5 px-2 rounded-lg text-xs font-semibold flex items-center justify-center gap-1.5 transition-all ${
                  viewMode === 'graph'
                    ? 'bg-gradient-to-r from-cyan-600 to-blue-600 text-white shadow-sm'
                    : 'text-slate-400 hover:text-slate-200'
                }`}
              >
                <Compass className="w-3.5 h-3.5" />
                Network Graph
              </button>
            </div>
          </div>

          {/* Tree Hierarchy Type Switcher (only relevant in tree mode) */}
          {viewMode === 'tree' && (
            <div>
              <label className="text-[11px] font-semibold text-slate-400 uppercase tracking-wider block mb-1.5">
                Tree Hierarchy Scheme
              </label>
              <div className="grid grid-cols-2 gap-1 p-1 rounded-xl bg-slate-900/80 border border-slate-800">
                <button
                  type="button"
                  onClick={() => setHierarchyType('academic')}
                  className={`py-1 px-2 rounded-lg text-[11px] font-medium transition-all ${
                    hierarchyType === 'academic'
                      ? 'bg-purple-900/70 text-purple-200 border border-purple-700/50'
                      : 'text-slate-400 hover:text-slate-200'
                  }`}
                >
                  Academic View
                </button>
                <button
                  type="button"
                  onClick={() => setHierarchyType('industry')}
                  className={`py-1 px-2 rounded-lg text-[11px] font-medium transition-all ${
                    hierarchyType === 'industry'
                      ? 'bg-purple-900/70 text-purple-200 border border-purple-700/50'
                      : 'text-slate-400 hover:text-slate-200'
                  }`}
                >
                  Industry View
                </button>
              </div>
            </div>
          )}

          {/* Active Subtree Banner */}
          {selectedNode && (
            <div className="p-2.5 rounded-xl bg-purple-950/40 border border-purple-800/50 flex items-center justify-between">
              <div>
                <span className="text-[10px] text-purple-300 font-semibold block uppercase">Active Subtree Branch</span>
                <span className="text-xs font-bold text-white truncate max-w-[170px] block">
                  {selectedNode.data?.label || selectedNode.id}
                </span>
              </div>
              <button
                onClick={onClearSelectedNode}
                className="p-1 rounded-lg text-purple-300 hover:text-white hover:bg-purple-900/60 transition-colors"
                title="Restore full tree"
              >
                <X className="w-4 h-4" />
              </button>
            </div>
          )}

          {/* Active Filter Chips */}
          {activeChips.length > 0 && (
            <div>
              <div className="flex items-center justify-between mb-1.5">
                <span className="text-[10px] uppercase font-semibold text-slate-400">Active Filters</span>
                <button
                  onClick={onResetFilters}
                  className="text-[10px] text-cyan-400 hover:underline flex items-center gap-1"
                >
                  <RotateCcw className="w-2.5 h-2.5" />
                  Clear All
                </button>
              </div>
              <div className="flex flex-wrap gap-1.5">
                {activeChips.map((chip, idx) => (
                  <span
                    key={idx}
                    className="inline-flex items-center gap-1 px-2 py-0.5 rounded-md bg-slate-800 text-cyan-300 border border-slate-700 text-[10px] font-medium"
                  >
                    {chip.label}
                    <button
                      type="button"
                      onClick={() => removeChip(chip)}
                      className="text-slate-400 hover:text-white"
                    >
                      <X className="w-3 h-3" />
                    </button>
                  </span>
                ))}
              </div>
            </div>
          )}

          {/* Keyword Search */}
          <div>
            <label className="text-[11px] font-semibold text-slate-400 uppercase tracking-wider block mb-1">
              Search Keywords
            </label>
            <div className="relative">
              <Search className="w-3.5 h-3.5 absolute left-2.5 top-2.5 text-slate-500" />
              <input
                type="text"
                placeholder="Search alumni, company, role..."
                value={filters.search || ''}
                onChange={(e) => handleInputChange('search', e.target.value)}
                className="w-full pl-8 pr-3 py-1.5 rounded-lg bg-slate-900/90 border border-slate-700/80 text-white placeholder-slate-500 focus:outline-none focus:border-cyan-400 text-xs"
              />
              {filters.search && (
                <button
                  onClick={() => handleInputChange('search', '')}
                  className="absolute right-2 top-2 text-slate-400 hover:text-white"
                >
                  <X className="w-3.5 h-3.5" />
                </button>
              )}
            </div>
          </div>

          {/* Batch Selector */}
          <div>
            <label className="text-[11px] font-semibold text-slate-400 uppercase tracking-wider block mb-1 flex items-center gap-1.5">
              <GraduationCap className="w-3.5 h-3.5 text-purple-400" />
              Graduation Batch
            </label>
            <select
              value={filters.batch_id || ''}
              onChange={(e) => handleInputChange('batch_id', e.target.value)}
              className="w-full px-2.5 py-1.5 rounded-lg bg-slate-900/90 border border-slate-700/80 text-white focus:outline-none focus:border-cyan-400 text-xs"
            >
              <option value="">All Batches</option>
              {metadata?.batches?.map((b) => (
                <option key={b.id} value={b.id}>
                  {b.degree} ({b.start_year} - {b.graduation_year})
                </option>
              ))}
            </select>
          </div>

          {/* Company Filter */}
          <div>
            <label className="text-[11px] font-semibold text-slate-400 uppercase tracking-wider block mb-1 flex items-center gap-1.5">
              <Building className="w-3.5 h-3.5 text-cyan-400" />
              Target Company
            </label>
            <select
              value={filters.company || ''}
              onChange={(e) => handleInputChange('company', e.target.value)}
              className="w-full px-2.5 py-1.5 rounded-lg bg-slate-900/90 border border-slate-700/80 text-white focus:outline-none focus:border-cyan-400 text-xs"
            >
              <option value="">All Companies</option>
              {metadata?.companies?.map((c) => (
                <option key={c.id} value={c.name}>
                  {c.name} ({c.industry})
                </option>
              ))}
            </select>
          </div>

          {/* Industry Filter */}
          <div>
            <label className="text-[11px] font-semibold text-slate-400 uppercase tracking-wider block mb-1 flex items-center gap-1.5">
              <Briefcase className="w-3.5 h-3.5 text-indigo-400" />
              Industry Sector
            </label>
            <select
              value={filters.industry || ''}
              onChange={(e) => handleInputChange('industry', e.target.value)}
              className="w-full px-2.5 py-1.5 rounded-lg bg-slate-900/90 border border-slate-700/80 text-white focus:outline-none focus:border-cyan-400 text-xs"
            >
              <option value="">All Industries</option>
              <option value="Technology & Internet">Technology & Internet</option>
              <option value="Enterprise Software & Cloud">Enterprise Software & Cloud</option>
              <option value="FinTech & Payments">FinTech & Payments</option>
              <option value="IT Consulting & Services">IT Consulting & Services</option>
              <option value="Consumer Tech & Logistics">Consumer Tech & Logistics</option>
              <option value="Investment Banking & Finance">Investment Banking & Finance</option>
            </select>
          </div>

          {/* Verification Status */}
          <div>
            <label className="text-[11px] font-semibold text-slate-400 uppercase tracking-wider block mb-1 flex items-center gap-1.5">
              <ShieldCheck className="w-3.5 h-3.5 text-emerald-400" />
              Profile Verification
            </label>
            <div className="grid grid-cols-3 gap-1">
              {[
                { id: '', label: 'All' },
                { id: 'verified', label: 'Verified' },
                { id: 'pending', label: 'Pending' },
              ].map((st) => (
                <button
                  key={st.id}
                  type="button"
                  onClick={() => handleInputChange('verification_status', st.id)}
                  className={`py-1 rounded-lg text-[10px] font-semibold transition-all ${
                    (filters.verification_status || '') === st.id
                      ? 'bg-emerald-950 text-emerald-300 border border-emerald-700'
                      : 'bg-slate-900/60 text-slate-400 hover:text-white border border-slate-800'
                  }`}
                >
                  {st.label}
                </button>
              ))}
            </div>
          </div>

          {/* Skills with AND / OR selector */}
          <div>
            <div className="flex items-center justify-between mb-1">
              <label className="text-[11px] font-semibold text-slate-400 uppercase tracking-wider">
                Skills Filter
              </label>
              <div className="inline-flex rounded-md bg-slate-900 p-0.5 border border-slate-800 text-[10px]">
                <button
                  type="button"
                  onClick={() => handleInputChange('skills_logic', 'or')}
                  className={`px-1.5 py-0.5 rounded font-mono font-bold ${
                    filters.skills_logic !== 'and'
                      ? 'bg-cyan-500 text-slate-950'
                      : 'text-slate-400 hover:text-white'
                  }`}
                  title="Match ANY selected skill"
                >
                  OR
                </button>
                <button
                  type="button"
                  onClick={() => handleInputChange('skills_logic', 'and')}
                  className={`px-1.5 py-0.5 rounded font-mono font-bold ${
                    filters.skills_logic === 'and'
                      ? 'bg-cyan-500 text-slate-950'
                      : 'text-slate-400 hover:text-white'
                  }`}
                  title="Match ALL selected skills"
                >
                  AND
                </button>
              </div>
            </div>

            <div className="flex flex-wrap gap-1 mt-1.5">
              {availableSkills.map((sk) => {
                const isSelected = filters.skills?.includes(sk);
                return (
                  <button
                    key={sk}
                    type="button"
                    onClick={() => handleSkillToggle(sk)}
                    className={`px-2 py-0.5 rounded-md text-[10px] font-mono transition-all ${
                      isSelected
                        ? 'bg-cyan-500 text-slate-950 font-bold border border-cyan-400 shadow-sm shadow-cyan-500/30'
                        : 'bg-slate-900/80 text-slate-400 hover:text-white border border-slate-800 hover:border-slate-700'
                    }`}
                  >
                    {sk}
                  </button>
                );
              })}
            </div>
          </div>

          {/* Reset Action */}
          <div className="pt-2">
            <button
              type="button"
              onClick={onResetFilters}
              className="w-full py-2 px-3 rounded-lg bg-slate-800/80 hover:bg-slate-700/80 text-slate-200 text-xs font-semibold flex items-center justify-center gap-2 border border-slate-700 transition-all"
            >
              <RotateCcw className="w-3.5 h-3.5 text-cyan-400" />
              Reset All Filters & Subtree
            </button>
          </div>
        </div>
      )}
    </aside>
  );
};

export default FilterSidebar;
