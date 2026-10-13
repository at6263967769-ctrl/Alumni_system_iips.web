import React, { useState, useEffect, useCallback } from 'react';
import {
  ReactFlow,
  Controls,
  MiniMap,
  Background,
  useNodesState,
  useEdgesState,
  BackgroundVariant,
} from '@xyflow/react';
import '@xyflow/react/dist/style.css';

import {
  Maximize2,
  RotateCcw,
  Route,
  Sparkles,
  AlertCircle,
  HelpCircle,
  Layers,
  Compass,
} from 'lucide-react';

import apiClient from '../api/client';
import { nodeTypes } from '../components/CustomNodes';
import FilterSidebar from '../components/FilterSidebar';
import AlumniDetailPanel from '../components/AlumniDetailPanel';
import ShortestPathModal from '../components/ShortestPathModal';

export const ExplorerPage = () => {
  const [nodes, setNodes, onNodesChange] = useNodesState([]);
  const [edges, setEdges, onEdgesChange] = useEdgesState([]);

  const [viewMode, setViewMode] = useState('tree'); // 'tree' | 'graph'
  const [hierarchyType, setHierarchyType] = useState('academic'); // 'academic' | 'industry'
  const [filters, setFilters] = useState({
    search: '',
    batch_id: '',
    company: '',
    industry: '',
    verification_status: '',
    skills: [],
    skills_logic: 'or',
  });

  const [metadata, setMetadata] = useState(null);
  const [allAlumniList, setAllAlumniList] = useState([]);
  const [selectedSubtreeNode, setSelectedSubtreeNode] = useState(null);
  const [activeAlumniDetail, setActiveAlumniDetail] = useState(null);
  const [isPathModalOpen, setIsPathModalOpen] = useState(false);

  const [isLoading, setIsLoading] = useState(false);
  const [apiError, setApiError] = useState(null);

  // Load initial metadata and alumni directory for dropdowns
  useEffect(() => {
    const fetchMeta = async () => {
      try {
        const metaRes = await apiClient.getMetadata();
        if (metaRes.status === 'success') {
          setMetadata(metaRes.data);
        }
        const alRes = await apiClient.getAlumniList({ limit: 100 });
        if (alRes.status === 'success') {
          setAllAlumniList(alRes.data.items || []);
        }
      } catch (err) {
        console.warn('Metadata load error:', err.message);
      }
    };
    fetchMeta();
  }, []);

  // Main data fetcher: queries C++ backend based on viewMode and filters
  const loadVisualizationData = useCallback(async () => {
    setIsLoading(true);
    setApiError(null);

    const queryParams = { ...filters };
    if (filters.skills?.length > 0) {
      queryParams.skills = filters.skills.join(',');
    } else {
      delete queryParams.skills;
    }

    try {
      let res;
      if (viewMode === 'tree') {
        if (selectedSubtreeNode) {
          res = await apiClient.getSubtree(selectedSubtreeNode.id, hierarchyType, queryParams);
        } else {
          res = await apiClient.getTree(hierarchyType, queryParams);
        }
      } else {
        // Graph mode
        res = await apiClient.getGraph(queryParams);
      }

      if (res.status === 'success') {
        const rfNodes = res.data.nodes || [];
        const rfEdges = res.data.edges || [];
        setNodes(rfNodes);
        setEdges(rfEdges);
      } else {
        setApiError(res.message || 'Failed to load graph visualization data.');
      }
    } catch (err) {
      setApiError(err.message || 'Error connecting to C++ backend server.');
    } finally {
      setIsLoading(false);
    }
  }, [viewMode, hierarchyType, filters, selectedSubtreeNode, setNodes, setEdges]);

  // Refetch when viewMode, hierarchy, filters, or selectedSubtreeNode changes
  useEffect(() => {
    loadVisualizationData();
  }, [loadVisualizationData]);

  // Handle node selection on canvas
  const handleNodeClick = useCallback(async (_, node) => {
    if (node.type === 'alumniNode') {
      const alumniData = node.data?.alumni || node.data;
      if (alumniData?.id || alumniData?.numeric_id) {
        const id = alumniData.id || alumniData.numeric_id;
        try {
          const res = await apiClient.getAlumniById(id);
          if (res.status === 'success') {
            setActiveAlumniDetail(res.data);
          } else {
            setActiveAlumniDetail(alumniData);
          }
        } catch {
          setActiveAlumniDetail(alumniData);
        }
      } else {
        setActiveAlumniDetail(alumniData);
      }
    } else if (node.type === 'hierarchyNode') {
      // Subtree isolation workflow: select branch to zoom in
      setSelectedSubtreeNode(node);
    }
  }, []);

  const handleResetFilters = () => {
    setFilters({
      search: '',
      batch_id: '',
      company: '',
      industry: '',
      verification_status: '',
      skills: [],
      skills_logic: 'or',
    });
    setSelectedSubtreeNode(null);
  };

  const handleClearSelectedNode = () => {
    setSelectedSubtreeNode(null);
  };

  const handleHighlightPath = (pathNodeIds) => {
    const idSet = new Set(pathNodeIds.map(String));
    setNodes((prev) =>
      prev.map((n) => ({
        ...n,
        selected: idSet.has(String(n.id)),
      }))
    );
    setEdges((prev) =>
      prev.map((e) => {
        const isPathEdge =
          idSet.has(String(e.source)) && idSet.has(String(e.target));
        return {
          ...e,
          animated: isPathEdge,
          style: {
            ...e.style,
            stroke: isPathEdge ? '#00D2FF' : '#334155',
            strokeWidth: isPathEdge ? 3 : 1,
          },
        };
      })
    );
  };

  return (
    <div className="flex h-[calc(100vh-4rem)] w-full overflow-hidden bg-[#080B12] text-slate-100 relative">
      {/* Left Filter & Subtree Sidebar */}
      <FilterSidebar
        viewMode={viewMode}
        setViewMode={setViewMode}
        hierarchyType={hierarchyType}
        setHierarchyType={setHierarchyType}
        filters={filters}
        setFilters={setFilters}
        onResetFilters={handleResetFilters}
        metadata={metadata}
        nodeCount={nodes.length}
        isLoading={isLoading}
        selectedNode={selectedSubtreeNode}
        onClearSelectedNode={handleClearSelectedNode}
      />

      {/* Main Canvas Area */}
      <main className="flex-1 h-full relative overflow-hidden flex flex-col">
        {/* Top Control Bar */}
        <div className="absolute top-4 left-4 right-4 z-10 flex flex-wrap items-center justify-between gap-3 pointer-events-none">
          <div className="pointer-events-auto flex items-center gap-2 p-1.5 rounded-xl glass-panel bg-slate-900/90 border border-slate-800 shadow-xl">
            <span className="text-[11px] font-bold uppercase tracking-wider text-slate-400 px-2 flex items-center gap-1.5">
              <span className="w-2 h-2 rounded-full bg-cyan-400 animate-pulse" />
              {viewMode === 'tree' ? 'Hierarchical DAG' : 'Adjacency Network'}
            </span>
            <span className="text-[10px] font-mono px-2 py-0.5 rounded bg-slate-800 text-cyan-300">
              {nodes.length} Nodes &bull; {edges.length} Edges
            </span>
          </div>

          <div className="pointer-events-auto flex items-center gap-2">
            <button
              onClick={() => setIsPathModalOpen(true)}
              className="px-3.5 py-2 rounded-xl bg-gradient-to-r from-cyan-500 to-indigo-600 hover:from-cyan-400 text-slate-950 font-bold text-xs flex items-center gap-1.5 shadow-lg shadow-cyan-500/25 transition-all"
            >
              <Route className="w-4 h-4 text-slate-950" />
              Find Shortest Path (BFS)
            </button>

            <button
              onClick={handleResetFilters}
              title="Reset graph view and clear filters"
              className="p-2 rounded-xl glass-panel bg-slate-900/90 hover:bg-slate-800 text-slate-300 border border-slate-700 shadow-lg transition-all"
            >
              <RotateCcw className="w-4 h-4" />
            </button>
          </div>
        </div>

        {/* Loading Indicator */}
        {isLoading && (
          <div className="absolute inset-0 z-20 bg-slate-950/60 backdrop-blur-sm flex items-center justify-center">
            <div className="p-4 rounded-2xl glass-panel flex items-center gap-3 text-cyan-400 border border-cyan-500/30">
              <Sparkles className="w-5 h-5 animate-spin" />
              <span className="text-xs font-semibold">Traversing C++ Graph Engine...</span>
            </div>
          </div>
        )}

        {/* API Error Banner */}
        {apiError && (
          <div className="absolute top-20 left-1/2 -translate-x-1/2 z-30 p-3 rounded-xl bg-rose-950/80 border border-rose-800/80 text-rose-200 text-xs flex items-center gap-2 shadow-xl">
            <AlertCircle className="w-4 h-4 text-rose-400 shrink-0" />
            <span>{apiError}</span>
            <button
              onClick={loadVisualizationData}
              className="ml-2 underline text-white font-bold"
            >
              Retry
            </button>
          </div>
        )}

        {/* Empty State */}
        {!isLoading && nodes.length === 0 && !apiError && (
          <div className="absolute inset-0 z-10 flex flex-col items-center justify-center text-center p-6 pointer-events-none">
            <div className="p-4 rounded-3xl bg-slate-900/80 border border-slate-800 max-w-sm pointer-events-auto">
              <Compass className="w-10 h-10 text-slate-600 mx-auto mb-3" />
              <h3 className="text-sm font-bold text-white mb-1">No Matching Subgraph Nodes</h3>
              <p className="text-xs text-slate-400 mb-4">
                The current combination of active filters pruned all tree branches.
              </p>
              <button
                onClick={handleResetFilters}
                className="w-full py-2 rounded-xl bg-cyan-600 hover:bg-cyan-500 text-slate-950 font-bold text-xs transition-all"
              >
                Reset All Filters
              </button>
            </div>
          </div>
        )}

        {/* React Flow Canvas */}
        <ReactFlow
          nodes={nodes}
          edges={edges}
          onNodesChange={onNodesChange}
          onEdgesChange={onEdgesChange}
          nodeTypes={nodeTypes}
          onNodeClick={handleNodeClick}
          fitView
          fitViewOptions={{ padding: 0.2 }}
          minZoom={0.15}
          maxZoom={2.0}
          defaultEdgeOptions={{
            type: 'smoothstep',
            animated: false,
          }}
        >
          <Background color="#1E293B" gap={24} size={1} variant={BackgroundVariant.Dots} />
          <Controls position="bottom-left" showInteractive={false} />
          <MiniMap
            position="bottom-right"
            nodeColor={(n) => (n.type === 'alumniNode' ? '#00D2FF' : '#9D00FF')}
            maskColor="rgba(8, 11, 18, 0.7)"
          />
        </ReactFlow>
      </main>

      {/* Node Detail Drawer Panel */}
      <AlumniDetailPanel
        alumni={activeAlumniDetail}
        onClose={() => setActiveAlumniDetail(null)}
        onSelectAlumniById={async (id) => {
          try {
            const res = await apiClient.getAlumniById(id);
            if (res.status === 'success') {
              setActiveAlumniDetail(res.data);
            }
          } catch {}
        }}
      />

      {/* Shortest Path Modal */}
      <ShortestPathModal
        isOpen={isPathModalOpen}
        onClose={() => setIsPathModalOpen(false)}
        alumniList={allAlumniList}
        onHighlightPath={handleHighlightPath}
      />
    </div>
  );
};

export default ExplorerPage;
