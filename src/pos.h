// Vertex positions with UV coordinates (first 2 = positions, last 2 = UV)
float VertexPositions[] = {
-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
 0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
};

//Vertex indices that will be used to draw trinagles, each row (3 indices) is one trinagle
unsigned int TriIndices[] = {
	0, 1, 2,
	2, 3, 0
};

// Vertex positions with UV coordinates (first 2 = positions, last 2 = UV)
float VertexPositions2[] = {
	 0.25f,  0.75f, -1.25f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	 0.25f,  0.25f, -1.25f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	 0.75f,  0.25f, -1.25f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	 0.75f,  0.75f, -1.25f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

	 0.25f,  0.75f, -2.75f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	 0.75f,  0.25f, -2.75f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	 0.25f,  0.25f, -2.75f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	 0.75f,  0.75f, -2.75f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,

	 0.25f,  0.75f, -1.25f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	 0.25f,  0.75f, -2.75f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	 0.25f,  0.25f, -2.75f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	 0.25f,  0.25f, -1.25f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

	 0.25f,  0.25f, -1.25f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	 0.25f,  0.25f, -2.75f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	 0.75f,  0.25f, -2.75f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	 0.75f,  0.25f, -1.25f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

};

//Vertex indices that will be used to draw trinagles, each row (3 indices) is one trinagle
unsigned int TriIndices2[] = {
	0, 1, 2,
	2, 3, 0,
	4, 5, 6,
	4, 7, 5,
	8, 9, 10,
	10, 11, 8,
	12, 13, 14,
	14, 15, 12,
};


// Vertex positions with UV coordinates (first 2 = positions, last 2 = UV)
float VertexPositions3[] = {
	-10.0f, 5.0f, -2.50f, 0.0f, 1.0f, 0.0f, 1.0f,
	-10.0f, 0.0f, -1.50f, 0.0f, 1.0f, 0.0f, 1.0f,
	10.0f, 5.0f, -2.50f, 0.0f, 1.0f, 0.0f, 1.0f,
	10.0f, 0.0f, -1.50f, 0.0f, 1.0f, 0.0f, 1.0f,

	-10.0f, 0.0f, -1.50f, 0.0f, 0.0f, 1.0f, 1.0f,
	-10.0f, -5.0f, -2.50f, 0.0f, 0.0f, 1.0f, 1.0f,
	10.0f, 0.0f, -1.50f, 0.0f, 0.0f, 1.0f, 1.0f,
	10.0f, -5.0f, -2.50f, 0.0f, 0.0f, 1.0f, 1.0f,

	0.0f, 10.0f, -1.50f, 0.75f, 0.75f, 1.0f, 1.0f,
	-5.0f, 10.0f, -2.50f, 0.75f, 0.75f, 1.0f, 1.0f,
	0.0f, -10.0f, -1.50f, 0.75f, 0.75f, 1.0f, 1.0f,
	-5.0f, -10.0f, -2.50f, 0.75f, 0.75f, 1.0f, 1.0f,

	0.0f, 10.0f, -1.50f, 0.5f, 0.5f, 0.0f, 1.0f,
	0.0f, -10.0f, -1.50f, 0.5f, 0.5f, 0.0f, 1.0f,
	5.0f, 10.0f, -2.50f, 0.5f, 0.5f, 0.0f, 1.0f,
	5.0f, -10.0f, -2.50f, 0.5f, 0.5f, 0.0f, 1.0f,
};

//Vertex indices that will be used to draw trinagles, each row (3 indices) is one trinagle
unsigned int TriIndices3[] = {
	0, 1, 2,
	2, 1, 3,

	4, 5, 6,
	6, 5, 7,

	8, 9, 10,
	10, 9, 11,

	12, 13, 14,
	14, 13, 15,

};