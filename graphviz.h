#ifndef GRAPHVIZ_H
#define GRAPHVIZ_H

#include "linked_list.h"

// цвета на графике
const char ZERO_FILL_COLOR[] = "#E0E0E0";
const char ZERO_COLOR[] = "#808080";

const char NODE_FILL_COLOR[] = "#E3F2FD";
const char NODE_COLOR[] = "#1E88E5";

const char HEAD_FILL_COLOR[] = "#E8F5E9";
const char HEAD_COLOR[] = "#4CAF50";

const char TAIL_FILL_COLOR[] = "#cb98a0";
const char TAIL_COLOR[] = "#62504f";

const char FREE_FILL_COLOR[] = "#FFF3E0";
const char FREE_COLOR[] = "#FB8C00";

const char ERROR_FILL_COLOR[] = "#f37d7d";
const char ERROR_COLOR[] = "#FF0000";

// const char HINT_FILL_COLOR[] = "#37474F";
// const char HINT_COLOR[] = "#37474F";
// const char HINT_FONT_COLOR[] = "#FFFFFF";
// const char HINT_EDGE_COLOR[] = "#37474F";

const char EDGE_COLOR[] = "#1E88E5";
const char FREE_EDGE_COLOR[] = "#E65100";
const char ERROR_EDGE_COLOR[] = "#FF0000";

char *draw_graph(LinkedList *list);

#endif // GRAPHVIZ_H