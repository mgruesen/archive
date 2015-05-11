﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace pa1
{
    class FFA
    {
        public BFS bfs;
        public Graph g;
        public FFA() 
        { 
            g = new Graph();
            bfs = new BFS(g);
        }
        public FFA(Graph g) 
        { 
            this.g = new Graph(g);
            bfs = new BFS(g);
        }

        public int DoSearch()
        {
            int ret = 0;
            int[] path;
            int cap_f;
            Graph resid = new Graph(g);
            resid.BuildResiduals();
            bfs.SetGraph(resid);
            //all edges should have an initial flow of 0 anyway, skipping this phase
            while (bfs.DoSearch() > 0)
            {
                //Console.WriteLine(ret);
                path = resid.GetPath().ToArray();
                ret += cap_f = getMinCap(path, resid.GetAdjacencyList());
                for (int i = 0; i < path.Length - 1; i++)
                {
                    int j = path[i];
                    int k = path[i+1];
                    if (g.ContainsEdge(j,k)){
                        resid.ChangeFlow(j, k, cap_f);
                        g.AddFlow(j, k, cap_f);
                    }
                    else
                        resid.ChangeFlow(k, j, -cap_f);
                }

                g.SetPath(resid.GetPath());
            }
            g.AddFlow(ret);
            return ret;
        }

        private int getMinCap(int[] path, List<Graph.Node> adjList)
        {
            int min = int.MaxValue;
            for (int i = 0; i < path.Length - 1; i++)
            {
                int val = adjList[path[i]].list[path[i + 1]]["capacity"];
                if (val < min)
                    min = val;
            }
            return min;
        }
    }
}