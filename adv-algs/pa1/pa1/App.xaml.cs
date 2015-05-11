using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace pa1
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        private static StreamReader sr;
        public static char[] delim = { ' ' };
        public static long nsPerTick = (1000L * 1000L * 1000L) / Stopwatch.Frequency; //From msdn.microsoft.com
        public struct Report
        {
            public int pathEdges;
            public int numEdges;
            public int numVertices;
            public int flow;
            public long timeMS;
            public long timeNS;
            public string path;
            
            /// <summary>
            /// 
            /// </summary>
            /// <param name="n">Number of Edges</param>
            /// <param name="v">Number of Vertices</param>
            /// <param name="f">Flow</param>
            /// <param name="tm">Time MS</param>
            /// <param name="tn">Time NS</param>
            /// <param name="pth">Path String</param>
            public Report(int n, int v, int f, long tm, long tn, string pth)
            {
                pathEdges = 0;
                numEdges = n;
                numVertices = v;
                flow = f;
                timeMS = tm;
                timeNS = tn;
                path = pth;
            }
            /// <summary>
            /// 
            /// </summary>
            /// <param name="p">Number of Path Edges</param>
            /// <param name="n">Number of Edges</param>
            /// <param name="v">Number of Vertices</param>
            /// <param name="f">Flow</param>
            /// <param name="tm">Time MS</param>
            /// <param name="tn">Time NS</param>
            /// <param name="pth">Path String</param>
            
            public Report(int p, int n, int v, int f, long tm, long tn, string pth) : this(n,v,f,tm,tn,pth)
            {
                pathEdges = p;
            }
        }

        [STAThread]
        public static void Main(String[] args)
        {
            Stopwatch watch = new Stopwatch();
            Graph g = null;
            int ret = -1;
            if (args.Length > 1)
            {
                g = processArgs(args);
                if (g == null) return;                    
                switch (args[0])
                {
                    case "-b"://BFS                        
                        BFS bfs = new BFS(g);
                        watch.Start();
                        ret = bfs.DoSearch();
                        watch.Stop();
                        g = new Graph(bfs.g);
                        break;
                    case "-f"://FFA
                        watch.Start();
                        FFA ffa = new FFA(g);
                        ret = ffa.DoSearch();
                        watch.Stop();
                        g = new Graph(ffa.g);
                        break;
                }
            }
            if (ret >= 0) { 
                Report rep = buildReport(g, ret, watch);
                System.Console.WriteLine("\n\nReport:\n");
                System.Console.WriteLine("From {0} to {1}", g.GetHead(), g.GetTail());
                System.Console.WriteLine(rep.path);
                System.Console.WriteLine("Edges: {0} Vertices: {1} Flow: {2} Time: {3}", rep.numEdges, rep.numVertices, rep.flow, rep.timeMS == 0 ? String.Format("{0} NS", rep.timeNS) : String.Format("{0} MS", rep.timeMS));
                System.Console.WriteLine("O(E+V)= {0}", rep.numEdges + rep.numVertices);                
                Pause();
            }
            Console.ReadKey(true);
        }

        public static Graph processArgs(string[] args)
        {
            Graph g = null;
            using (StreamReader sr = new StreamReader(args[1])) 
            { 
                String line;                
                switch (args[0])
                {
                    case "-b":
                    case "-f":
                        try
                        {
                            g = new Graph();
                            int lineCount = 0;
                            while ((line = sr.ReadLine()) != null)
                            {
                                g.CreateNode(lineCount);
                                if (line.Trim().Length > 0)
                                {
                                    string[] temp = line.Split(delim);
                                    for (int j = 0; j < temp.Length; j += 2)
                                    {
                                        g.AddNodeTo(lineCount, Convert.ToInt32(temp[j]), Convert.ToInt32(temp[j + 1]));
                                    }
                                }
                                lineCount++;
                            }
                            g.CreateNode(lineCount);
                            int h = Convert.ToInt32(args[2]);
                            int t = Convert.ToInt32(args[3]);
                            int max = g.GetBaseNodeCount();
                            if (h < 0 || h >= max)
                            {
                                throw new ArgumentOutOfRangeException("Source", String.Format("Source must be between {0} and {1} exclusive", 0, max));
                            }
                            if (t < 0 || t >= max)
                            {
                                throw new ArgumentOutOfRangeException("Sink", String.Format("Sink must be between {0} and {1} exclusive", 0, max));
                            }
                            g.SetHead(h);
                            g.SetTail(t);                                               
                    }
                    catch (Exception e)
                    {
                        System.Console.WriteLine(e.Message);
                        Pause();
                        g = null;
                        break;
                    }
                    break;//case b/f        
                    case "-m":
                        try
                        {
                            string[] temp;
                            Stopwatch watch;
                            int numWallSets;
                            int numArt;
                            int numGuards;
                            int numWalls;
                            int problem = 0;
                            //first line; numWallSets, numArt, numGuards
                            StreamWriter sw = new StreamWriter(args[2]);
                            while ((line = sr.ReadLine()) != "0 0 0")
                            {
                                g = new Graph();
                                temp = line.Split(delim, 3);
                                numWallSets = Convert.ToInt32(temp[0]);
                                numArt = Convert.ToInt32(temp[1]);
                                numGuards = Convert.ToInt32(temp[2]);
                                //wall set parser
                                for (int j = 0; j < numWallSets; j++)
                                {
                                    g.AddWallSet();
                                    line = sr.ReadLine();
                                    numWalls = Convert.ToInt32((line.Split(delim))[0]);
                                    for (int k = 0; k < numWalls; k++)
                                    {
                                        line = sr.ReadLine();
                                        temp = line.Split(delim);
                                        int x = Convert.ToInt32(temp[0]);
                                        int y = Convert.ToInt32(temp[1]);
                                        Graph.LineType type = temp[2] == "s" ? Graph.LineType.STRAIGHT : Graph.LineType.CURVE;
                                        if (type == Graph.LineType.STRAIGHT)
                                        {
                                            g.AddWallToSet(j, new Graph.Wall(x, y, type));
                                        }
                                        else
                                        {
                                            int dx = Convert.ToInt32(temp[3]);
                                            int dy = Convert.ToInt32(temp[4]);
                                            g.AddWallToSet(j, new Graph.CurvedWall(x, y, type, dx, dy));
                                        }
                                    }
                                }
                                //art parser
                                for (int j = 0; j < numArt; j++)
                                {
                                    line = sr.ReadLine();
                                    temp = line.Split(delim);
                                    int x = Convert.ToInt32(temp[0]);
                                    int y = Convert.ToInt32(temp[1]);
                                    int val = Convert.ToInt32(temp[2]);
                                    g.AddArt(new Graph.Art(x, y, val));
                                }
                                //guard parser
                                for (int j = 0; j < numGuards; j++)
                                {
                                    line = sr.ReadLine();
                                    temp = line.Split(delim);
                                    int x = Convert.ToInt32(temp[0]);
                                    int y = Convert.ToInt32(temp[1]);
                                    int w = Convert.ToInt32(temp[2]);
                                    g.AddGuard(new Graph.Guard(x, y, w));
                                }
                                watch = new Stopwatch();
                                MainWindow mw = StartGUI(g);
                                watch.Start();
                                g.museumToGraph();
                                FFA ffa = new FFA(g);
                                int flow = ffa.DoSearch();
                                int sum = g.artSum();
                                Console.WriteLine("flow: " + flow + ", required: " + g.artSum() + ", solution? " + (flow == sum));
                                watch.Stop();                                
                                g.drawMuseum();
                                mw.Show();
                                g.PrintAdjList();
                                sw.WriteLine("Case {0}: {1}", ++problem, (flow == sum ? "Yes" : "No"));
                                if (flow == sum)
                                {
                                    string l;
                                    List<Graph.Node> adjList = g.GetAdjacencyList();
                                    for (int i = 0; i < g.GetGuards().Count; i++)
                                    {
                                        l = "";
                                        l += String.Format("G{0}: ", i+1);
                                        foreach(int j in adjList[i].list.Keys)                                        
                                        {
                                            if (adjList[i].list[j]["flow"] > 0)
                                            {
                                                l += String.Format("A{0}, ", j);
                                            }
                                        }
                                        l = l.TrimEnd(new char[]{',', ' '});
                                        sw.WriteLine(l);
                                    }
                                }
                                Report rep = buildReport(ffa.g, 0, watch);
                                System.Console.WriteLine("\nReport:\n");
                                System.Console.WriteLine("From {0} to {1}", g.GetHead(), g.GetTail());
                                System.Console.WriteLine(rep.path);
                                System.Console.WriteLine("Edges: {0} Vertices: {1} Flow: {2} Time: {3}", rep.numEdges, rep.numVertices, rep.flow, rep.timeMS == 0 ?String.Format("{0} NS", rep.timeNS) : String.Format("{0} MS", rep.timeMS));
                                System.Console.WriteLine("O(E+V)= {0}", rep.numEdges + rep.numVertices);
                                Console.ReadKey(true);
                                mw.Close();                                                                                                  
                            }//end while non-term 
                            sw.Close();
                        }
                        catch (Exception e)
                        {
                            Console.WriteLine(e.Message);
                            Console.WriteLine(e.StackTrace);
                            g = null;
                        }
                        break;//case m
                }//end using                
            }
            return g;
        }

        public static MainWindow StartGUI(Graph g)
        {
            return new MainWindow(g);
        }
        public static Report buildReport(Graph g, int ret, Stopwatch watch)
        {
            string path = "";
            int[] list = g.GetPath().ToArray();
            int head = g.GetHead();
            int tail = g.GetTail();         
            for (int i = list.Length - 1; i >= 0; i-- )
            {
                if (i > 0)                
                    path = path.Insert(0, String.Format(" => {0}", list[i]));
                else
                    path = path.Insert(0, String.Format("{0}", list[i]));
            }
            //path = path.Insert(0, String.Format("{0}", head));
            int totalEdges = 0;
            List<Graph.Node> adjList = g.GetAdjacencyList();
            for (int i = 0; i < adjList.Count; i++)
            {
                totalEdges += adjList[i].list.Count;
            }
            Report rep = new Report(ret, totalEdges, adjList.Count, g.GetFlow(), watch.ElapsedMilliseconds, watch.ElapsedTicks * nsPerTick, path);
            return rep;
        }
        public static void Pause() { System.Console.ReadKey(true); }
    }
}
