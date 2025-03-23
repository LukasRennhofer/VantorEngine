"""
*    				~ CHIFEngine ~
*               
* Copyright (c) 2025 Lukas Rennhofer
*
* Licensed under the MIT License. See LICENSE file for more details.
*
* Author: Lukas Rennhofer
* Date: 03-23-2025
*
* File: plotArchitecture.py
* Last Change: Tooling Updatet
"""

import networkx as nx
import matplotlib.pyplot as plt

# Create a directed graph
G = nx.DiGraph()

# Adding nodes
G.add_nodes_from([
    "chif",
    "Entity",
    "EntityBase",
    "ComponentBase",
    "Mesh",
    "Model",
    "Camera",
    "Network",
    "Server",
    "Client",
    "Core",
    "Version",
    "Resource Management",
    "Application Handling",
    "Job System",
    "BackLog System",
    "Graphics",
    "Renderer",
    "Renderer::Shader",
    "Renderer::Buffer",
    "Water",
    "Terrain",
    "Texture",
    "VolumetricClouds",
    "VFX",
    "GUI",
    "Sprite",
    "AnimatedSprite",
    "Physics",
    "Utils",
    "Platform",
    "Platform::Window",
    "Audio"
])

# Adding edges (relationships between nodes)
G.add_edges_from([
    ("chif", "Entity"),
    ("Entity", "EntityBase"),
    ("Entity", "ComponentBase"),
    ("Entity", "Mesh"),
    ("Entity", "Model"),
    ("Entity", "Camera"),
    ("chif", "Network"),
    ("Network", "Server"),
    ("Network", "Client"),
    ("chif", "Core"),
    ("Core", "Version"),
    ("Core", "Resource Management"),
    ("Core", "Application Handling"),
    ("Core", "Job System"),
    ("Core", "BackLog System"),
    ("chif", "Graphics"),
    ("Graphics", "Renderer"),
    ("Renderer", "Renderer::Shader"),
    ("Renderer", "Renderer::Buffer"),
    ("Graphics", "Water"),
    ("Graphics", "Terrain"),
    ("Graphics", "Texture"),
    ("Graphics", "VolumetricClouds"),
    ("Graphics", "VFX"),
    ("chif", "GUI"),
    ("GUI", "Sprite"),
    ("GUI", "AnimatedSprite"),
    ("chif", "Physics"),
    ("chif", "Utils"),
    ("chif", "Platform"),
    ("Platform", "Platform::Window"),
    ("chif", "Audio")
])

# Draw the graph
plt.figure(figsize=(12, 12))
pos = nx.spring_layout(G, seed=42)  # Positions for all nodes
nx.draw(G, pos, with_labels=True, node_size=3000, node_color="lightblue", font_size=10, font_weight="bold", edge_color="gray")
plt.title("System Architecture Tree", fontsize=15)
plt.show()