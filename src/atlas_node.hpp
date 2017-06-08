//
//  atlas_node.hpp
//  atlas-tree-xcode
//
//  Created by Hsu Chaohung on 2017/06/08.
//  Copyright © 2017年 hsu. All rights reserved.
//

#ifndef atlas_node_hpp
#define atlas_node_hpp

#include <memory>

namespace hsu {

struct rect {
    int x, y, width, height;
};

struct image {
    int width, height;
};

class atlas_node : public std::enable_shared_from_this<atlas_node> {
public:
    atlas_node(int width, int height);
    
    std::shared_ptr<atlas_node const> insert(image const& img);
    inline rect const& rect() const { return rc_; }
    inline bool is_image() const { return is_image_; }

private:
    std::shared_ptr<atlas_node> left_node_;
    std::shared_ptr<atlas_node> right_node_;
    struct rect rc_;
    bool is_image_;
};

} // end of namespace hsu

#endif /* atlas_node_hpp */
