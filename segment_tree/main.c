#include "segment_tree.h"

int main(void) {
  Seg_tree *sgt = init_seg_tree_empty(16);
  point_update_seg_tree(sgt, 0, 10);
  point_update_seg_tree(sgt, 1, 11);
  point_update_seg_tree(sgt, 2, 12);
  point_update_seg_tree(sgt, 3, 13);
  point_update_seg_tree(sgt, 4, 14);
  point_update_seg_tree(sgt, 5, 15);
  point_update_seg_tree(sgt, 6, 16);
  point_update_seg_tree(sgt, 7, 17);
    
  return 0;
}
