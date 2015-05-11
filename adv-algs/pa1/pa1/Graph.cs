using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace pa1
{
    /// <summary>
    /// Graph class; Uses and adjacency list to represent the nodes and the weight
    /// of their edges.
    /// </summary>

    public class Graph
    {
        public const int DEFAULT_WEIGHT = 0;

        public enum Color { WHITE, GRAY, BLACK };
        public enum LineType { STRAIGHT, CURVE };
        #region Inner Classes
        public class XYVAL
        {
            public int x, y, val;
            public XYVAL(int x, int y, int v)
            {
                this.x = x;
                this.y = y;
                val = v;
            }
        }
        public class Art : XYVAL
        {
            public Art(int x, int y, int v) : base(x, y, v) { }
        }

        public class Guard : XYVAL
        {
            public Guard(int x, int y, int w) : base(x, y, w) { }
        }

        public interface IWall
        {
            int GetX();
            int GetY();
            LineType GetType();
            void Print();
        }

        public class Wall : IWall
        {
            public LineType line;
            public int x, y;

            public Wall(int x, int y, LineType type)
            {
                this.x = x;
                this.y = y;
                line = type;
            }
            public int GetX() { return x; }

            public int GetY() { return y; }

            public new LineType GetType() { return line; }

            public virtual void Print()
            {
                Console.WriteLine("{0} {1} {2}", x, y, 's');
            }
        }

        public class CurvedWall : Wall
        {
            public int dx, dy;
            public CurvedWall(int x, int y, LineType type, int dx, int dy)
                : base(x, y, type)
            {
                this.dx = dx;
                this.dy = dy;
            }

            public int GetDX() { return dx; }
            public int GetDY() { return dy; }

            public override void Print()
            {
                Console.WriteLine("{0} {1} {2} {3} {4}", x, y, 'c', dx, dy);
            }
        }
        /// <summary>
        /// Node Class. Has an ID, distance, prev & next links, color, and weight.
        /// </summary>
        public class Node
        {
            public int id;
            public int dist;
            public int prev;
            public Color color;
            public Dictionary<int, Dictionary<string, int>> list;
            public Node()
            {
                throw new ArgumentException("Must provide an ID");
            }
            public Node(int i)
            {
                id = i;
                dist = 0;
                prev = -1;
                color = Color.WHITE;
                list = new Dictionary<int, Dictionary<string, int>>();
            }

            public Node(int i, int d)
                : this(i)
            {
                dist = d;
            }

            public Node(int i, int d, int p)
                : this(i, d)
            {
                prev = p;
            }

            public Node(int i, int d, int p, Dictionary<int, Dictionary<string, int>> l)
                : this(i, d, p)
            {
                list = l;
            }

            public void Clear()
            {
                foreach (Dictionary<string, int> li in list.Values)
                {
                    li["flow"] = 0;
                }
            }
        }

        public class Edge
        {
            public int u, v;
            public int cap;
            public int flow;
            public Edge(int u, int v, int c)
            {
                this.u = u;
                this.v = v;
                cap = c;
                flow = 0;
            }

            public void Reset() { flow = 0; }

            public bool Equals(Edge o)
            {
                return this.u == o.u && this.v == o.v;
            }
        }
        #endregion

        public int minX = 0, maxX = 1, minY = 0, maxY = 1;

        //Adjacency List
        private List<Node> adjList;
        private List<List<IWall>> walls;
        private List<Guard> guards;
        private List<Art> art;
        private List<Edge> edges;
        private Stack<int> path;
        private int head, tail;
        private string outputFile;
        private int totalFlow = 0;

        public Graph()
        {
            adjList = new List<Node>();
            walls = new List<List<IWall>>();
            guards = new List<Guard>();
            art = new List<Art>();
            edges = new List<Edge>();
            path = new Stack<int>();
        }
        public Graph(Graph g)
        {
            this.adjList = new List<Node>();
            this.adjList.AddRange(g.adjList);
            this.walls = new List<List<IWall>>();
            this.walls.AddRange(g.walls);
            this.guards = new List<Guard>();
            this.guards.AddRange(g.guards);
            this.art = new List<Art>();
            this.art.AddRange(g.art);
            this.edges = new List<Edge>();
            this.edges.AddRange(g.edges);
            path = g.path;
            head = g.head;
            tail = g.tail;
            totalFlow = g.totalFlow;
        }

        // Maintains a bounding box that contains all of the walls, guards, and art.
        private void updateBounds(int x, int y)
        {
            if (x < minX) minX = x;
            if (x > maxX) maxX = x;
            if (y < minY) minY = y;
            if (y > maxY) maxY = y;
        }
        #region Adders
        public void AddWallSet()
        {
            walls.Add(new List<IWall>());
        }

        public void AddWallToSet(int i, IWall wall)
        {
            walls[i].Add(wall);
            updateBounds(wall.GetX(), wall.GetY());
        }

        public void AddArt(Art a)
        {
            art.Add(a);
            updateBounds(a.x, a.y);
        }

        public void AddGuard(Guard g)
        {
            guards.Add(g);
            updateBounds(g.x, g.y);
        }

        public void BuildResiduals()
        {
            foreach (Node n in adjList)
            {
                foreach (int i in n.list.Keys)
                {
                    AddNodeTo(i, n.id);
                }
            }
        }
        public void ChangeFlow(int i, int j, int flow)
        {
            adjList[i].list[j]["flow"] += flow;            
            adjList[i].list[j]["capacity"] -= flow;            
        }
        public void AddFlow(int i, int j, int flow)
        {
            adjList[i].list[j]["flow"] += flow;
        }

        private void removeEdge(int i, int j)
        {
            adjList[i].list.Remove(j);
        }
        public void ChangeCapacity(int i, int j, int cap)
        {
            adjList[i].list[j]["capacity"] = cap;
        }

        public bool ContainsEdge(int i, int j)
        {
            return adjList[i].list.ContainsKey(j);
        }

        /// <summary>
        /// Adds a new base node to the adjacency list and sorts them by ID in Ascending order
        /// </summary>
        /// <param name="i"></param>
        public void CreateNode(int i)
        {
            adjList.Add(new Node(i));
            adjList.Sort(baseComparer);
        }
        /// <summary>
        /// Adds a node to a base node's adjacency list with a default weight
        /// </summary>
        /// <param name="i"></param>
        /// <param name="n"></param>
        public void AddNodeTo(int i, int n)
        {
            AddNodeTo(i, n, DEFAULT_WEIGHT);
        }

        /// <summary>
        /// Adds a node to a base node's adjacency list IF it does not already exist
        /// </summary>
        /// <param name="i">Base Node ID</param>
        /// <param name="n">New Node</param>
        public void AddNodeTo(int i, int n, int w)
        {
            if (!nodeContains(i, n))
            {
                Dictionary<string, int> temp = new Dictionary<string, int>();
                temp.Add("capacity", w);
                temp.Add("flow", 0);
                adjList[i].list.Add(n, temp);
            }
        }
        #endregion
        #region Clearers
        /// <summary>
        /// Clears the adjacency list
        /// </summary>
        public void ClearNodes()
        {
            adjList.Clear();
        }

        public void ClearGuards()
        {
            guards.Clear();
        }

        public void ClearArt()
        {
            art.Clear();
        }

        public void ClearWalls()
        {
            walls.Clear();
        }
        #endregion
        #region Getters
        public Stack<int> GetPath() { return path; }
        /// <summary>
        /// Gets the number of base nodes in the adjacency list
        /// </summary>
        /// <returns>Adjacency List Count</returns>
        public int GetBaseNodeCount() { return adjList.Count; }

        public List<Edge> GetEdgeList() { return edges; }

        public List<Node> GetAdjacencyList() { return adjList; }
        public List<List<IWall>> GetWalls() { return walls; }
        public List<Guard> GetGuards() { return guards; }
        public List<Art> GetArt() { return art; }
        public int GetHead() { return head; }
        public int GetTail() { return tail; }
        public int GetFlow() { return totalFlow; }
        
        #endregion
        #region Setters
        public void SetHead(int h) { head = h; }
        public void SetTail(int t) { tail = t; }
        public void SetOutputFile(String f) { outputFile = f; }
        public void SetPath(Stack<int> p) { path = p; }
        public void AddFlow(int amount) { totalFlow += amount; }
        #endregion
        #region Printers
        public void Print()
        {
            PrintAdjList();
            Console.WriteLine("{0} {1} {2}", walls.Count, art.Count, guards.Count);
            PrintWallList();
            PrintArtList();
            PrintGuardList();
        }

        /// <summary>
        /// Prints the adjacency list
        /// </summary>
        public void PrintAdjList()
        {
            Console.WriteLine("Adjacency List:");
            //foreach (Node n in adjList)
            for (int i = 0; i < adjList.Count; i++)
            {
                Console.Write("{0}: ", i);
                foreach (KeyValuePair<int, Dictionary<string, int>> kvp in adjList[i].list)
                {
                    Console.Write("=>{0},{1} ", kvp.Key, kvp.Value["capacity"]);
                }
                Console.WriteLine();
            }
            Console.WriteLine();
        }
        public void PrintGuardList()
        {
            Console.WriteLine("Guards:");
            foreach (Guard g in guards)
            {
                Console.WriteLine("{0} {1} {2}", g.x, g.y, g.val);
            }
            Console.WriteLine();
        }

        public void PrintArtList()
        {
            Console.WriteLine("Art:");
            foreach (Art a in art)
            {
                Console.WriteLine("{0} {1} {2}", a.x, a.y, a.val);
            }
            Console.WriteLine();
        }

        public void PrintWallList()
        {
            Console.WriteLine("Walls:");
            foreach (List<IWall> ws in walls)
            {
                Console.WriteLine("{0}", ws.Count);
                foreach (IWall w in ws)
                {
                    w.Print();
                }
            }
            Console.WriteLine();
        }
        #endregion
        #region Util
        /// <summary>
        /// Comparer for Base Nodes; Compares IDs
        /// </summary>
        /// <param name="x">BaseNode X</param>
        /// <param name="y">BaseNode Y</param>
        /// <returns>-1 : x.id < y.id; 0 : x.id == y.id; 1 : x.id > y.id</returns>
        private int baseComparer(Node x, Node y)
        {
            return x.id.CompareTo(y.id);
        }
        /// <summary>
        /// Comparere for Edges; Compares ID of u at each edge, then ID of v is a.u == b.u
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <returns></returns>
        private int edgeComparer(Edge a, Edge b)
        {
            if (a.u == b.u)
            {
                return a.v.CompareTo(b.v);
            }
            else return a.u.CompareTo(b.u);
        }
        /// <summary>
        /// Checks if the given node is in the base node's adjacency list
        /// </summary>
        /// <param name="i">Base Node ID</param>
        /// <param name="n">Potential New Node</param>
        /// <returns></returns>
        private bool nodeContains(int i, int n)
        {
            return adjList[i].list.ContainsKey(n);
        }
        #endregion

        // Checks if the point (x2,y2) lies on the segment from (x1,y1) to (x3,y3)
        // Points are presumed to be colinear.
        // Calculation is done by checking that both endpoints are closer to
        // point 2 than to each other.
        private bool liesBetween(int x1, int y1, double x2, double y2, int x3, int y3)
        {
            double d12 = (y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1);
            double d23 = (y3 - y2) * (y3 - y2) + (x3 - x2) * (x3 - x2);
            double d13 = (y3 - y1) * (y3 - y1) + (x3 - x1) * (x3 - x1);
            return d12 < d13 && d23 < d13;
        }

        private System.Windows.Point circleCenter(double x2, double y2, double dx, double dy, double x3, double y3)
        {
            double m2;
            if (dx == 0) m2 = double.PositiveInfinity;
            else m2 = dy / dx;
            double m2Perp, b2Perp;
            if (double.IsPositiveInfinity(m2)) m2Perp = double.Epsilon;
            else if (m2 == 0) m2Perp = double.PositiveInfinity;
            else m2Perp = -1 / m2;
            b2Perp = y2 - (m2Perp * x2);

            double xMid = ((double)x2 + x3) / 2;
            double yMid = ((double)y2 + y3) / 2;
            double mMidPerp, bMidPerp;
            if (xMid == x2) mMidPerp = double.Epsilon;
            else if (yMid == y2) mMidPerp = double.PositiveInfinity;
            else mMidPerp = -1 / ((yMid - y2) / (xMid - x2));
            bMidPerp = yMid - (mMidPerp * xMid);

            double xCenter, yCenter;

            if (x2 == x3) xCenter = x3;
            else if (double.IsPositiveInfinity(m2Perp)) xCenter = x2;
            else xCenter = (bMidPerp - b2Perp) / (m2Perp - mMidPerp);
            if (y2 == y3)
            {
                yCenter = y3;
                xCenter = xMid;
            }
            else yCenter = m2Perp * xCenter + b2Perp;

            return new System.Windows.Point(xCenter, yCenter);
        }

        private double findAngle(double xCenter, double yCenter, double xPerim, double yPerim, double radius)
        {
            double angle = 0;
            if (xPerim > xCenter)
            {
                angle = Math.Acos((xPerim - xCenter) / radius);
                if (yPerim < yCenter) angle = Math.PI * 2 - angle;
            }
            else
            {
                angle = Math.Acos((xPerim - xCenter) / radius);
                if (yPerim < yCenter) angle = Math.PI * 2 - angle;
            }

            return angle;
        }

        // Determines if a is visible to g given the wall configuration
        public bool isVisible(Guard g, Art a)
        {
            //Console.WriteLine("a,g: " + a.x + " " + a.y + " " + g.x + " " + g.y);
            double m1;  // Slope of the line from the guard to the art
            if (a.x == g.x) m1 = double.PositiveInfinity;  // Vertical line
            else m1 = ((double)(a.y - g.y)) / (a.x - g.x);
            double b1 = (double)g.y - m1 * g.x;  // Y-intercept
            // Check each wall for obstruction. Return false if any are in the way,
            // or true if the list is exhausted.
            foreach (List<IWall> ws in walls)
            {
                int x2, y2, x3, y3;
                double m2, b2;
                for (int i = 0; i < ws.Count; i++ )
                {
                    if (ws[i].GetType() == LineType.CURVE)
                    {
                        CurvedWall curve = (CurvedWall)ws[i];
                        x2 = curve.GetX();
                        y2 = curve.GetY();
                        double dx = curve.GetDX();
                        double dy = curve.GetDY();
                        x3 = ws[(i + 1) % ws.Count].GetX();
                        y3 = ws[(i + 1) % ws.Count].GetY();

                        System.Windows.Point center = circleCenter(x2, y2, dx, dy, x3, y3);
                        double xCenter = center.X;
                        double yCenter = center.Y;

                        double radius = Math.Sqrt((x2 - xCenter) * (x2 - xCenter) + (y2 - yCenter) * (y2 - yCenter));

                        double cPrime = b1 + m1 * xCenter - yCenter;
                        double intersects = radius * radius * (m1 * m1 + 1) - cPrime * cPrime;
                        //Console.WriteLine("Intersects?" + intersects + " " + xCenter + " " + yCenter);
                        if (intersects <= 0) continue;

                        double d = intersects / (m1 * m1 + 1);
                        double xInt1 = -m1 * cPrime + d;
                        double xInt2 = -m1 * cPrime - d;
                        double yInt1 = cPrime + m1 * d;
                        double yInt2 = cPrime - m1 * d;
                        //Console.WriteLine(xInt1 + " " + yInt1 + " " + xInt2 + " " + yInt2);

                        double startAngle = findAngle(xCenter, yCenter, x2, y2, radius);
                        double endAngle = findAngle(xCenter, yCenter, x3, y3, radius);
                        double intAngle1 = findAngle(xCenter, yCenter, xInt1, yInt1, radius);
                        double intAngle2 = findAngle(xCenter, yCenter, xInt2, yInt2, radius);

                        if (liesBetween(g.x, g.y, xInt1, yInt1, a.x, a.y))
                        {
                            //Console.WriteLine(startAngle + " " + intAngle1 + " " + intAngle2 + " " + endAngle);
                            if (startAngle < intAngle1 && intAngle1 < endAngle) return false;
                            if (endAngle < intAngle1 && intAngle1 < startAngle) return false;
                        }
                        if (liesBetween(g.x, g.y, xInt2, yInt2, a.x, a.y))
                        {

                            //Console.WriteLine(startAngle + " " + intAngle1 + " " + intAngle2 + " " + endAngle);
                            if (startAngle < intAngle2 && intAngle2 < endAngle) return false;
                            if (endAngle < intAngle2 && intAngle2 < startAngle) return false;
                        }
                        continue;
                    }
                    x2 = ws[i].GetX();
                    y2 = ws[i].GetY();
                    x3 = ws[(i+1) % ws.Count].GetX();
                    y3 = ws[(i+1) % ws.Count].GetY();
                    if (x3 == x2)  // Vertical wall
                    {
                        if (double.IsPositiveInfinity(m1)) continue;  // If both lines are vertical it can't block view
                        double yIntersect = m1 * x2 + b1;
                        if (liesBetween(g.x, g.y, x2, yIntersect, a.x, a.y) &&
                            liesBetween(x2, y2, x2, yIntersect, x3, y3)) return false;
                        else continue;
                    }
                    m2 = ((double)(y3 - y2)) / (x3 - x2);  // Slope of wall
                    b2 = y2 - m2 * x2;  // Y-intercept of wall
                    if (m1 == m2) continue; // Guaranteed not to intersect at endpoints/for parallel lines
                    double xIntersection;  // Intersection of the wall and the guard-art line
                    if (double.IsPositiveInfinity(m1)) xIntersection = g.x;
                    else xIntersection = (b2 - b1) / (m1 - m2);
                    double yIntersection = m2 * xIntersection + b2;
                    // Confirm that the intersection is on the segment of the line containing the wall
                    if (liesBetween(g.x, g.y, xIntersection, yIntersection, a.x, a.y) &&
                        liesBetween(x2, y2, xIntersection, yIntersection, x3, y3))
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        // Converts a museum layout to a bipartite flow network with
        // guards in one partite set and art in the other, along with
        // a supersource and supersink, with edges weighted such that
        // a maximum flow represents a solution to the "Watch, Man"
        // problem.
        public void museumToGraph()
        {
            this.CreateNode(0);  // s
            this.SetHead(0);
            int t = this.guards.Count + this.art.Count + 1;
            this.CreateNode(t); //t
            this.SetTail(t);
            int i = 1;
            int j = this.guards.Count + 1;
            foreach (Guard g in this.guards)
            {
                this.CreateNode(i);
                this.AddNodeTo(0, i, g.val);
                i++;
            }
            foreach (Art a in this.art)
            {
                this.CreateNode(j);
                this.AddNodeTo(j, t, a.val);
                j++;
            }
            i = 1;
            foreach (Guard g in this.guards)
            {
                j = this.guards.Count + 1;
                foreach (Art a in this.art)
                {
                    if (isVisible(g, a))
                    {
                        this.AddNodeTo(i, j, 1);
                    }
                    j++;
                }
                i++;
            }
        }

        public int artSum()
        {
            int sum = 0;
            foreach (Art a in art)
            {
                sum += a.val;
            }
            return sum;
        }

        // Converts a museum coordinate to the corresponding window coordinate.
        // Takes into account the fact that WPF uses an inverted y-axis.
        public double toScreen(double val, double scale, double offset, double invert)
        {
            double answer = val * scale + offset;
            return invert == 0 ? answer : invert - answer;
        }

        // Renders a museum layout to the screen. Walls are shown as black lines/arcs,
        // art is shown as red squares, and guards as green circles.
        public void drawMuseum()
        {
            System.Windows.Controls.Canvas grid = MainWindow.baseGrid;
            double height = grid.Height;
            double xScale = grid.Width / (2.0 * (maxX - minX));
            double yScale = grid.Height / (2.0 * (maxY - minY));
            double scale = Math.Min(xScale, yScale);
            double offset = -Math.Min(minX, minY) + 100;
            //Console.WriteLine(scale + " " + offset);

            foreach (List<IWall> ws in walls)
            {
                for (int i = 0; i < ws.Count; i++)
                {
                    if (ws[i].GetType() == LineType.CURVE)
                    {
                        System.Windows.Shapes.Path p = new System.Windows.Shapes.Path();
                        CurvedWall curve = (CurvedWall)ws[i];
                        double x2 = curve.GetX();
                        double y2 = curve.GetY();
                        double dx = curve.GetDX();
                        double dy = curve.GetDY();
                        double x3 = ws[(i + 1) % ws.Count].GetX();
                        double y3 = ws[(i + 1) % ws.Count].GetY();

                        System.Windows.Point center = circleCenter(x2, y2, dx, dy, x3, y3);
                        double xCenter = toScreen(center.X, scale, offset, 0);
                        double yCenter = toScreen(center.Y, scale, offset, height);
                        double startX = toScreen(x2, scale, offset, 0);
                        double startY = toScreen(y2, scale, offset, height);
                        double endX = toScreen(x3, scale, offset, 0);
                        double endY = toScreen(y3, scale, offset, height);
                        double radius = Math.Sqrt((startX - xCenter) * (startX - xCenter) + (startY - yCenter) * (startY - yCenter));

                        double x4 = x2 + dx;
                        double y4 = y2 + dy;

                        double det = x4 * y3 + x2 * y4 + x3 * y2 - y2 * x4 - y4 * x3 - y3 * x2;

                        int clockwise = det < 0 ? 1 : 0;
                        //Console.WriteLine(x2 + " " + x3 + " " + x4 + " " + e32 + " " + y2 + " " + y3);

                        //Console.WriteLine(string.Format("M{0},{1} A{2},{2} 0 0 {3} {4},{5} {6} {7} {8}", startX, startY, radius, clockwise, endX, endY, dx, dy, det));
                        p.Data = Geometry.Parse(string.Format("M{0},{1} A{2},{2} 0 0 {3} {4},{5}", startX, startY, radius, clockwise, endX, endY));
                        p.Stroke = Brushes.Black;
                        p.StrokeThickness = 2;
                        grid.Children.Add(p);
                    }
                    else
                    {
                        System.Windows.Shapes.Line l = new System.Windows.Shapes.Line();
                        l.X1 = toScreen(ws[i].GetX(), scale, offset, 0);
                        l.Y1 = toScreen(ws[i].GetY(), scale, offset, height);
                        l.X2 = toScreen(ws[(i + 1) % ws.Count].GetX(), scale, offset, 0);
                        l.Y2 = toScreen(ws[(i + 1) % ws.Count].GetY(), scale, offset, height);
                        l.Stroke = Brushes.Black;
                        l.StrokeThickness = 2;
                        grid.Children.Add(l);
                    }
                }
            }

            foreach (Guard g in guards)
            {
                System.Windows.Shapes.Rectangle l = new System.Windows.Shapes.Rectangle();
                l.Stroke = Brushes.DarkCyan;
                l.StrokeThickness = 5;
                l.Width = 3;
                l.Height = 3;
                grid.Children.Add(l);
                //Console.WriteLine(toScreen(g.y, scale, offset, height));
                System.Windows.Controls.Canvas.SetTop(l, toScreen(g.y, scale, offset, height) - 1);
                System.Windows.Controls.Canvas.SetLeft(l, toScreen(g.x, scale, offset, 0) - 1);
            }

            foreach (Art a in art)
            {
                System.Windows.Shapes.Rectangle l = new System.Windows.Shapes.Rectangle();
                l.Stroke = Brushes.Red;
                l.StrokeThickness = 2;
                l.Width = 3;
                l.Height = 3;
                grid.Children.Add(l);
                System.Windows.Controls.Canvas.SetTop(l, toScreen(a.y, scale, offset, height) - 1);
                System.Windows.Controls.Canvas.SetLeft(l, toScreen(a.x, scale, offset, 0) - 1);
            }
            
        }


    }
}
