using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using pa1;

namespace test
{
    class Program
    {
        static void Main(string[] args)
        {

            Graph g = new Graph();
            g.AddWallSet();
            g.AddWallToSet(0, new Graph.CurvedWall(0, 0, Graph.LineType.CURVE, 1, 1));
            g.AddWallToSet(0, new Graph.CurvedWall(2, 1, Graph.LineType.CURVE, 1, -1));
            Graph.Guard guard = new Graph.Guard(-1, 1, 10);
            g.AddGuard(guard);
            Graph.Art art = new Graph.Art(3, 2, 2);
            g.AddArt(art);
            Console.WriteLine(g.isVisible(guard, art));
            Graph.Art art2 = new Graph.Art(21, -1, 1);
            //g.AddArt(art2);
            //Console.WriteLine(g.isVisible(guard, art2));
            Console.ReadLine();
            g.museumToGraph();
            g.PrintAdjList();
            Console.ReadLine();

            Graph g2 = new Graph();
            g2.AddWallSet();
            g2.AddWallToSet(0, new Graph.Wall(0, 0, Graph.LineType.STRAIGHT));
            g2.AddWallToSet(0, new Graph.Wall(0, 10, Graph.LineType.STRAIGHT));
            g2.AddWallToSet(0, new Graph.Wall(20, 10, Graph.LineType.STRAIGHT));
            g2.AddWallToSet(0, new Graph.Wall(20, 0, Graph.LineType.STRAIGHT));
            g2.AddGuard(guard);
            g2.AddArt(art);
            Console.WriteLine(g2.isVisible(guard, art));
            g2.AddArt(art2);
            Console.WriteLine(g2.isVisible(guard, art2));
            Graph.Art art3 = new Graph.Art(5, 1, 3);
            g2.AddArt(art3);
            Console.WriteLine(g2.isVisible(guard, art3));
            Console.ReadLine();

            g2.museumToGraph();
            g2.PrintAdjList();
            Console.ReadLine();
        }
    }
}
