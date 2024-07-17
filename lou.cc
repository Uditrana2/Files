#include "comm3.h"
#include <vector>
#include <unordered_map>
#include <algorithm>

auto LouvainCommunityDetection(graph& g)
{
    int num_nodes = g.num_nodes();
    std::vector<int> community(num_nodes);
    std::vector<float> degree(num_nodes, 0);

    #pragma omp parallel for
    for (int v = 0; v < num_nodes; v++) 
    {
        community[v] = v;
        degree[v] = g.indexofNodes[v+1] - g.indexofNodes[v];
    }

    long long totalEdges = 0;
    #pragma omp parallel for reduction(+:totalEdges)
    for (int v = 0; v < num_nodes; v++) 
    {
        totalEdges += degree[v];
    }
    totalEdges /= 2;

    bool improvement = true;
    int iterations = 0;
    const int maxIterations = 19;

    std::vector<int> new_community(num_nodes);

    while (improvement && iterations < maxIterations) {
        improvement = false;
        iterations++;

        #pragma omp parallel for
        for (int v = 0; v < num_nodes; v++) 
        {
            int old_community = community[v];
            int best_community = old_community;
            double best_gain = 0.0;

            std::unordered_map<int, int> community_connections;
            for (int edge = g.indexofNodes[v]; edge < g.indexofNodes[v+1]; edge++) 
            {
                int nbr = g.edgeList[edge];
                community_connections[community[nbr]]++;
            }

            for (const auto& entry : community_connections) 
            {
                int nbr_community = entry.first;
                int edge_count = entry.second;
                
                if (nbr_community != old_community) 
                {
                    double gain = edge_count - (degree[v] * degree[nbr_community]) / (2.0 * totalEdges);
                    if (gain > best_gain) 
                    {
                        best_gain = gain;
                        best_community = nbr_community;
                    }
                }
            }

            new_community[v] = best_community;
            if (best_community != old_community) 
            {
                improvement = true;
            }
        }

        #pragma omp parallel for
        for (int v = 0; v < num_nodes; v++)
        {
            community[v] = new_community[v];
        }
    }

    std::unordered_map<int, bool> seen_communities;
    int num_communities = 0;

    for (int v = 0; v < num_nodes; v++) 
    {
        if (seen_communities.find(community[v]) == seen_communities.end()) 
        {
            seen_communities[community[v]] = true;
            num_communities++;
        }
    }

    return num_communities;
}
