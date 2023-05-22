#pragma once

enum Primitive {
	Line,
	Triangle
};

typedef struct Point2D
{
	float x;
	float y;
} Point2D;

typedef struct Line2D {
	Point2D start;
	Point2D end;
} Line2D;


enum VertexLayout {
	Vertex,
	VertexNormal,
	VertexColor,
	VertexNormalColor
};

typedef struct VertexBuffer {
	std::vector<float> vertice;
	std::vector<unsigned int> indices;
	VertexLayout layout;
} VertexBuffer;

