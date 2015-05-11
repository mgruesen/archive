using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO;
using pa1;
using System.Diagnostics;

namespace bfs_test
{
    class Program
    {
        public static char[] delim = { ' ' };
        public static long nsPerTick = (1000L * 1000L * 1000L) / Stopwatch.Frequency; //From msdn.microsoft.com
        public static int runBFSTest(BFS b)
        {
            return b.DoSearch();
        }

        public static void Pause() { Console.ReadKey(true); }

        public static Graph processArgs(string[] args)
        {
            String line;
            Graph g = new Graph();
            if(args[0] == "-b"){
                try
                {
                    using (StreamReader sr = new StreamReader(args[1]))
                    {
                        int lineCount = 0;
                        while ((line = sr.ReadLine()) != null)
                        {
                            g.CreateNode(lineCount);
                            if (line.Trim().Length > 0) {
                                string[] temp = line.Split(delim);
                                for (int j = 0; j < temp.Length; j += 2) {
                                    g.AddNodeTo(lineCount, Convert.ToInt32(temp[j]), Convert.ToInt32(temp[j+1]));
                                }
                            }
                            lineCount++;
                        }
                        g.SetHead(Convert.ToInt32(args[2]));
                        g.SetTail(Convert.ToInt32(args[3]));
                    }
                }
                catch (Exception e)
                {
                    System.Console.WriteLine(e.Message);
                    System.Console.WriteLine(e.StackTrace);
                }
            }
            else if (args[0] == "-m"){
                try
                {
                    using (StreamReader sr = new StreamReader(args[1]))
                    {
                        //first line; numWallSets, numArt, numGuards
                        line = sr.ReadLine();
                        string[] temp = line.Split(delim, 3);
                        int numWallSets = Convert.ToInt32(temp[0]);
                        int numArt = Convert.ToInt32(temp[1]);
                        int numGuards = Convert.ToInt32(temp[2]);
                        //wall set parser
                        for (int j = 0; j < numWallSets; j++)
                        {
                            g.AddWallSet();
                            line = sr.ReadLine();
                            int numWalls = Convert.ToInt32((line.Split(delim))[0]);                                    
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
                        g.Print();
                    }
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.Message);
                    Console.WriteLine(e.StackTrace);
                }
            }
            return g;
        }

        static void Main(string[] args)
        {
            Stopwatch watch = new Stopwatch();
            watch.Start();
            Graph g = null;
            int ret = -1;
            if (args.Length > 1) 
            {
                g = processArgs(args);
                switch(args[0]){//should be flag
                    case "-b":
                        //bfs test
                        BFS bfs = new BFS(g);
                        ret = runBFSTest(bfs);
                    break;
                    case "-f":
                    //ford-fulk test
                    break;
                    case "-m":
                    //museum test
                    break;
                }
            }
            System.Console.WriteLine(String.Format("{0}", ret));
            watch.Stop();

            string path = "";
            List<Graph.Node> list = g.GetAdjacencyList();
            int head = g.GetHead();
            int tail = g.GetTail();
            Graph.Node current = list[tail];
            while (current.id != head)
            {
                path = path.Insert(0, String.Format(" => {0}", current.id));
                current = list[current.prev];
            }
            path = path.Insert(0, String.Format("{0}", head));
            int totalEdges = 0;
            for (int i = 0; i < list.Count; i++)
            {
                totalEdges += list[i].list.Count;
            }
            BFS.Report rep = new BFS.Report(ret, totalEdges, list.Count, watch.ElapsedMilliseconds, watch.ElapsedTicks * nsPerTick, path);
            System.Console.WriteLine("\n\nReport:\n");
            System.Console.WriteLine("From {0} to {1}", head, tail);
            System.Console.WriteLine(rep.path);
            System.Console.WriteLine("Edges: {0} Vertices: {1} Time:{2}", rep.numEdges, rep.numVertices, rep.timeMS == 0 ? String.Format("{0} NS", rep.timeNS) : String.Format("{0} MS", rep.timeMS));
            System.Console.WriteLine("Projected Worst Runtime: {0}", rep.numEdges + rep.numVertices);            
            Pause();
        }
    }
}
