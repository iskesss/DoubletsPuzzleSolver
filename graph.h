#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <cassert>
#include <vector>
#include <list>
#include <queue>

using namespace std;

class graph
{
public:

    // default constructor
    // pre: none
    // post: creates an empty graph
    graph()
    {
        _v = vector<list<size_t>>();
    }

    // pre: none
    // post: returns the number of vertices in this graph
    size_t n() const
    {
        return _v.size();
    }

    // pre: none
    // post: returns the number of edges in this graph
    size_t m() const
    {
        size_t ans(0);
        for (auto e: _v)
            ans += e.size();

        return ans/2;
    }

    // pre: none
    // post: a new vertex (numbered with an integer) has
    //       been added to this graph
    void add_vertex()
    {
        _v.push_back(list<size_t>());
    }


    // pre: a & b are existing vertices
    // post: edge {a b} has been added to this graph
    //       if it was not in this graph; otherwise do nothing
    void add_edge(size_t a, size_t b)
    {
        assert(a < n() && b < n());
        if (!is_edge(a, b))
        {
            _v[a].push_front(b);
            _v[b].push_front(a);
        }
    }

    // pre: a is an existing vertex
    // post: vertex a and associated edges with a as an endpoint
    //       has been removed from this graph
    void remove_vertex(size_t a);

    // pre: a, b are existing vertices
    // post: edge {a, b} has been removed from this graph if it
    //       was in this graph; otherwise do nothing
    void remove_edge(size_t a, size_t b)
    {
        assert(a < n() && b < n());
        if (is_edge(a, b))
        {
            _v[a].remove(b);
            _v[b].remove(a);
        }
    }

    // pre: none
    // post: returns true iff {a, b} is an edge in this graph
    bool is_edge(size_t a, size_t b) const
    {
        if (a >= n() || b >= n())
            return false;

        for (auto e: _v[a])
            if (e == b)
                return true;

        return false;
    }
    list<size_t> neighbors (size_t a) const
    {
        assert(a < n());
        return _v[a];
    }

    size_t ncc()
    {
        vector<int> parent(n(), -1);
        size_t connected_components(0);
	size_t dia(0);
	size_t w;

        for (size_t start = 0; start < n(); ++start)
        {
            if (parent[start] == -1)
            {
                bfs1(start, parent);
                ++connected_components;
		if (diameter(start) > dia)
		{
			w = start;
			dia = diameter(start);
		}
            }
        }
	cout << "diameter = " << dia << " at " << w << endl;
        return connected_components;
    }

    bool is_connected()
    {
        return (ncc() == 1);

    }

    bool is_cyclic() const;
    bool is_tree() const;
    bool is_bipartite() const;

    bool is_complete() const
    {
        for (auto e: _v)
            if (e.size() != n()-1)
                return false;

        return true;
    }

    size_t diameter(size_t start)
    {
	    queue<size_t> q;
	    vector<int> d(n(), -1);
	    q.push(start);
	    d[start] = 0;
	    size_t f;
	    while (!q.empty())
	    {
		    f = q.front();
		    q.pop();
		    for (auto e: _v[f])
			    if (d[e] == -1)
			    {
				    d[e] = d[f] + 1;
				    q.push(e);

			    }
	    }
	    cout << "diameter starting at " << f;
	    size_t newstart = f;
	    q.push(f);
	    d = vector<int>(n(), -1);
	    d[f] = 0;
	    while(!q.empty())
	    {
		    f = q.front();
		    q.pop();
		    for (auto e: _v[f])
			    if (d[e] == -1)
			    {
				    d[e] = d[f] + 1;
				    q.push(e);
			    }
	    }
	    cout << "  ending at " << f << ": " << d[f] << endl;

	    for (auto e: shortest_path(newstart, f))
		    cout << e << " ";
	    cout << "  len = " << d[f] << endl;
	    

	    return d[f];
    }

    vector<int> shortest_path(size_t start, size_t dest)
    {

	    vector<int> parent(n(), -1);
	    bfs1(start, parent);

	    if (parent[dest] == -1)
		    return vector<int>();

	    vector<int> ans;
	    for (int v = dest; v != start; v = parent[v])
	    {
		    ans.push_back(v);
	    }
	    ans.push_back(start);
	    return ans;
    }


private:

    void bfs1(size_t start, vector<int> & parent)
    {
        queue<size_t> q;

        parent[start] = start;
        q.push(start);

        while(!q.empty())
        {
            size_t f = q.front();
            q.pop();
            for (auto e: _v[f])
                if (parent[e] == -1)
                {
                    parent[e] = f;
                    q.push(e);
                }
        }
    }


    vector<list<size_t>> _v;

    // invariants:
    // n = number of vertices
    // m = number of edges
    // vertices are consecutively number 0, 1, ..., n-1
    // _v[i] is an (unsorted) list of neighbors of i
    //  (vertices adjacent to i)

};


ostream & operator <<(ostream & os, const graph & g)
{
    os << g.n() << " " << g.m() << endl;
    for (size_t i = 0; i < g.n(); ++i)
    {
        list<size_t> n = g.neighbors(i);
        for (auto e: n)
            if (e > i)
                os << i << " " << e << endl;
    }
    return os;
}

istream & operator >>(istream & is, graph & g)
{
    size_t n, m, a, b;

    is >> n >> m;

    g = graph();
    for (size_t i = 0; i < n; ++i)
        g.add_vertex();

    for (size_t i = 0; i < m; ++i)
    {
        is >> a >> b;
        g.add_edge(a, b);
    }
    return is;


}

#endif // GRAPH_H
