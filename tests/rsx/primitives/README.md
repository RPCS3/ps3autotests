The main loop of this sample is equivalent to this code. Following the order of these lines, the objects are drawn from the bottom row to the top row, and for each row, from left to right. All vertices are stored in a vertex buffer, and the appropriate index for each element is passed to `cellGcmSetDrawArrays`.

```
// Draw Points (thick)
cellGcmSetPointSize(5.0);
cellGcmSetDrawArrays(CELL_GCM_PRIMITIVE_POINTS, 0, 4);
cellGcmSetPointSize(1.0);

// Draw Lines (stippled)
cellGcmSetLineStippleEnable(CELL_GCM_TRUE);
cellGcmSetLineStipplePattern(&linePattern, 3);
cellGcmSetDrawArrays(CELL_GCM_PRIMITIVE_LINES, 4, 4);
cellGcmSetLineStippleEnable(CELL_GCM_FALSE);

// Draw Line Strip (thick)
cellGcmSetLineWidth(5*8); // 5.0
cellGcmSetDrawArrays(CELL_GCM_PRIMITIVE_LINE_STRIP, 8, 4);
cellGcmSetLineWidth(1*8); // 5.0

// Draw Line Loop (smooth)
cellGcmSetLineSmoothEnable(CELL_GCM_TRUE);
cellGcmSetDrawArrays(CELL_GCM_PRIMITIVE_LINE_LOOP, 12, 4);
cellGcmSetLineSmoothEnable(CELL_GCM_FALSE);

// Draw Triangles and Quads
cellGcmSetDrawArrays(CELL_GCM_PRIMITIVE_TRIANGLES, 16, 8);
cellGcmSetDrawArrays(CELL_GCM_PRIMITIVE_TRIANGLE_STRIP, 24, 8);
cellGcmSetDrawArrays(CELL_GCM_PRIMITIVE_TRIANGLE_FAN, 32, 8);
cellGcmSetDrawArrays(CELL_GCM_PRIMITIVE_QUAD_STRIP, 40, 8);
cellGcmSetDrawArrays(CELL_GCM_PRIMITIVE_QUADS, 48, 8);

// Draw Polygon (fill)
cellGcmSetBackPolygonMode(CELL_GCM_POLYGON_MODE_FILL);
cellGcmSetDrawArrays(CELL_GCM_PRIMITIVE_POLYGON, 78, 6);

// Draw Polygon (fill+smooth)
cellGcmSetBackPolygonMode(CELL_GCM_POLYGON_MODE_FILL);
cellGcmSetPolySmoothEnable(CELL_GCM_TRUE);
cellGcmSetDrawArrays(CELL_GCM_PRIMITIVE_POLYGON, 84, 6);
cellGcmSetPolySmoothEnable(CELL_GCM_FALSE);

// Draw Polygons (points / line / fill+stipple)
cellGcmSetBackPolygonMode(CELL_GCM_POLYGON_MODE_POINT);
cellGcmSetDrawArrays(CELL_GCM_PRIMITIVE_POLYGON, 90, 6);
cellGcmSetBackPolygonMode(CELL_GCM_POLYGON_MODE_LINE);
cellGcmSetDrawArrays(CELL_GCM_PRIMITIVE_POLYGON, 96, 6);
cellGcmSetBackPolygonMode(CELL_GCM_POLYGON_MODE_FILL);
cellGcmSetPolygonStippleEnable(CELL_GCM_TRUE);
cellGcmSetPolygonStipplePattern(polyPattern);
cellGcmSetDrawArrays(CELL_GCM_PRIMITIVE_POLYGON, 102, 6);
cellGcmSetPolygonStippleEnable(CELL_GCM_FALSE);
```

The vertex buffer is initialized with following function. Note that some vertices won't be used.

```
void setVertex(vertex_t* V) {
	size_t i = 0;

	// Points, Lines, Triangles, Quads: [rows #1, #2, #3]
	for (int y=0; y<3; y++) {
		for (int x=0; x<6; x++) {
			V[i].x = -1.0f+0.1*(x+1)+0.2*(x+0.0);  V[i].y = -1.0f+0.15*(y+1)+0.3*(y+0.0);  V[i].z = -1.0f;  V[i].rgba=0x00ff0000;  i++;
			V[i].x = -1.0f+0.1*(x+1)+0.2*(x+1.0);  V[i].y = -1.0f+0.15*(y+1)+0.3*(y+0.0);  V[i].z = -1.0f;  V[i].rgba=0x0000ff00;  i++;
			V[i].x = -1.0f+0.1*(x+1)+0.2*(x+0.0);  V[i].y = -1.0f+0.15*(y+1)+0.3*(y+1.0);  V[i].z = -1.0f;  V[i].rgba=0xff000000;  i++;
			V[i].x = -1.0f+0.1*(x+1)+0.2*(x+1.0);  V[i].y = -1.0f+0.15*(y+1)+0.3*(y+1.0);  V[i].z = -1.0f;  V[i].rgba=0x00000000;  i++;
		}
	}

	// Polygons: [rows #3 #4]
	for (int y=2; y<4; y++) {
		for (int x=0; x<3; x++) {
			V[i].x = -1.0f+0.3*(x+1)+0.3*(x+0.0);  V[i].y = -1.0f+0.15*(y+1)+0.3*(y+1.0);  V[i].z = -1.0f;  V[i].rgba=0xff000000;  i++;
			V[i].x = -1.0f+0.3*(x+1)+0.3*(x+1.0);  V[i].y = -1.0f+0.15*(y+1)+0.3*(y+1.0);  V[i].z = -1.0f;  V[i].rgba=0x00ff0000;  i++;
			V[i].x = -1.0f+0.3*(x+1)+0.3*(x+1.2);  V[i].y = -1.0f+0.15*(y+1)+0.3*(y+0.5);  V[i].z = -1.0f;  V[i].rgba=0x0000ff00;  i++;
			V[i].x = -1.0f+0.3*(x+1)+0.3*(x+1.0);  V[i].y = -1.0f+0.15*(y+1)+0.3*(y+0.0);  V[i].z = -1.0f;  V[i].rgba=0xffff0000;  i++;
			V[i].x = -1.0f+0.3*(x+1)+0.3*(x+0.0);  V[i].y = -1.0f+0.15*(y+1)+0.3*(y+0.0);  V[i].z = -1.0f;  V[i].rgba=0x00ffff00;  i++;
			V[i].x = -1.0f+0.3*(x+1)+0.3*(x-0.2);  V[i].y = -1.0f+0.15*(y+1)+0.3*(y+0.5);  V[i].z = -1.0f;  V[i].rgba=0xff00ff00;  i++;
		}
	}
}
```