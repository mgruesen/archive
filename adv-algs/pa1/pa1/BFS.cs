using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace pa1
{
    public class BFS
    {        
        public struct Report
        {
            public int pathEdges;
            public int numEdges;
            public int numVertices;
            public long timeMS;
            public long timeNS;
            public string path;
            public Report(int p, int n, int v, long tm, long tn, string pth)
            {
                pathEdges = p;
                numEdges = n;
                numVertices = v;
                timeMS = tm;
                timeNS = tn;
                path = pth;
            }
        }

        public Graph g;
        private Queue<int> q;
        private int head, tail;
        
        public BFS()
        {
            g = new Graph();
            q = new Queue<int>();
        }
        
        public BFS(Graph g)
        {
            this.g = new Graph(g);
            q = new Queue<int>();
            head = g.GetHead();
            tail = g.GetTail();
        }
        public BFS(Graph g, int h, int t) : this(g)
        {
            head = h;
            tail = t;
        }

        public void SetHead(int h) { head = h; }
        public void SetTail(int t) { tail = t; }

        public int GetHead() { return head; }
        public int GetTail() { return tail; }

        public void SetGraph(Graph g) { this.g = g; }

        public int DoSearch()
        {         
            q.Clear();
            List<Graph.Node> list = g.GetAdjacencyList();
            foreach (Graph.Node n in list)
            {
                n.prev = -1;
                n.dist = 0;
                n.color = Graph.Color.WHITE;
            }
            q.Enqueue(head);
            while (q.Count > 0)
            {
                Graph.Node u = list[q.Dequeue()];
                Graph.Node v;
                foreach (int i in u.list.Keys)
                {
                    v = list[i];
                    if (v.color == Graph.Color.WHITE && u.list[i]["capacity"] > 0)
                    {
                        v.color = Graph.Color.GRAY;
                        v.dist = u.dist + 1;
                        v.prev = u.id;
                        q.Enqueue(v.id);
                        list[i] = v;
                    }                    
                }
                list[u.id].color = Graph.Color.BLACK;
            }
            Graph.Node current = list[tail];
            Stack<int> path = new Stack<int>();
            path.Push(tail);
            int edges = 0;
            while (current.prev >= 0)
            {
                current = list[current.prev];
                edges++;
                path.Push(current.id);
            }
            //path.Push(current.id);
            g.SetPath(path);
            return edges; 
        }

        public void Print()
        {
            List<Graph.Node> list = g.GetAdjacencyList();
            Graph.Node current = list[list.Count - 1];
            int tail = g.GetTail();
            while (current.id != tail)
            {
                System.Console.Write("{0}=>", current.id);
                current = list[current.prev];
            }
            System.Console.WriteLine("{0}", tail);
        }
    }
}
