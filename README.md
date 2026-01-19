# Artifact-Researcher-Management-System-AVL-Red-Black-Trees-
This project is a C++-based database management system built on self-balancing tree data structures. The main objective is to implement the internal mechanics of AVL Trees and Red-Black Trees from scratch, including rotations, balancing logic, and memory management.

About the Project

The system manages historical artifacts and the researchers working on them. To maximize efficiency, a hybrid approach using two different tree structures is employed:
	•	AVL Tree (Artifacts):
Artifacts are ordered by their IDs. Due to its strict balancing rules, the AVL Tree provides maximum performance for search operations.
	•	Red-Black Tree (Researchers):
Researchers are ordered by their names. Since Red-Black Trees require fewer rotations during insert and delete operations compared to AVL Trees, they are preferred for dynamic datasets.

Technical Details

1. Data Structures

The Standard Template Library (STL) is not used. All node structures, pointer connections, and balancing algorithms are implemented manually.
	•	AVL Tree:
Uses RotateLeft and RotateRight operations based on the balance factor (height difference).
	•	Red-Black Tree:
Applies InsertFixup and DeleteFixup algorithms according to red/black coloring rules.

2. Management Module

ArtifactManager processes text-based commands and coordinates operations between the two trees:
	•	HIRE_RESEARCHER: Inserts a new node into the Red-Black Tree.
	•	ADD_ARTIFACT: Inserts a new artifact into the AVL Tree.
	•	REQUEST: Searches for an artifact (AVL search) and assigns it to a researcher (RB search).

File Structure
	•	AVLTree.h / .cpp: AVL Tree implementation for storing artifacts.
	•	RedBlackTree.h / .cpp: Red-Black Tree implementation for storing researchers.
	•	ArtifactManager.h / .cpp: Command parser and system controller.
	•	main.cpp: Main entry point that reads input from a file.

Build & Run
g++ -o ArtifactSystem main.cpp ArtifactManager.cpp AVLTree.cpp RedBlackTree.cpp Artifact.cpp Researcher.cpp
./ArtifactSystem input.txt
