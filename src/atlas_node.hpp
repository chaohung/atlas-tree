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
#include <functional>

namespace hsu {

class atlas_node : public std::enable_shared_from_this<atlas_node> {
public:
    struct rect {
        int x, y, width, height;
    };

public:
    static std::shared_ptr<atlas_node> create_atlas_root(int width, int height);

public:
    std::shared_ptr<atlas_node const> insert(int img_width, int img_height, int margin = 0);
    void resize(int new_width, int new_height);
    void clear();

    void traverse(std::function<void(std::shared_ptr<atlas_node const>)> handler) const;
    inline std::shared_ptr<atlas_node const> left_node() const { return left_node_; }
    inline std::shared_ptr<atlas_node const> right_node() const { return right_node_; }
    inline rect const& rect() const { return rc_; }
    inline bool is_image() const { return is_image_; }

private:
    atlas_node(int width, int height);
    std::shared_ptr<atlas_node> insert_impl(int img_width, int img_height);

    template<typename T>
    struct make_shared_helper : public T {
        template<typename... Args>
        explicit make_shared_helper(Args&&... args) : T(std::forward<Args>(args)...) {}
    };

public:
    std::function<void(int old_width, int old_height, int new_width, int new_height)> resize_handler;

private:
    std::shared_ptr<atlas_node> left_node_;
    std::shared_ptr<atlas_node> right_node_;
    struct rect rc_;
    bool is_image_;
};

} // end of namespace hsu

#endif /* atlas_node_hpp */
