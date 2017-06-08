//
//  atlas_node.cpp
//  atlas-tree-xcode
//
//  Created by Hsu Chaohung on 2017/06/08.
//  Copyright © 2017年 hsu. All rights reserved.
//

#include "atlas_node.hpp"

namespace hsu {

std::shared_ptr<atlas_node> atlas_node::create_atlas_root(int width, int height) {
    return std::make_shared<make_shared_helper<atlas_node> >(width, height);
}

std::shared_ptr<atlas_node const> atlas_node::insert(int img_width, int img_height) {
    auto img_node = insert_impl(img_width, img_height);

    if (!img_node || (rc_.width == img_width && rc_.height == img_height) ) {
        auto new_width = rc_.width + img_width;
        auto new_height = rc_.height + img_height;
        resize(new_width, new_height);
        return insert(img_width, img_height);
    }
    return img_node;
}

void atlas_node::resize(int new_width, int new_height) {
    if (left_node_ && right_node_) {
        if (new_height <= rc_.width || new_height <= rc_.height) return;
    }

    auto tmp_rc = rc_;
    auto tmp_left_node = left_node_;
    auto tmp_right_node = right_node_;
    rc_ = {0, 0, new_width, new_height};
    clear();

    if (tmp_rc.width > 0 && tmp_rc.height > 0) {
        auto img_node = insert_impl(tmp_rc.width, tmp_rc.height);
        if (img_node) {
            img_node->is_image_ = false;
            img_node->left_node_ = tmp_left_node;
            img_node->right_node_ = tmp_right_node;
        }
    }
    if (resize_handler) {
        resize_handler(new_width, new_height);
    }
}

void atlas_node::clear() {
    is_image_ = false;
    left_node_ = nullptr;
    right_node_ = nullptr;
}

void atlas_node::traverse(std::function<void(std::shared_ptr<atlas_node const>)> handler) const {
    if (left_node_) {
        left_node_->traverse(handler);
    }
    if (right_node_) {
        right_node_->traverse(handler);
    }
    // post-order
    handler(shared_from_this());
}

std::shared_ptr<atlas_node> atlas_node::insert_impl(int img_width, int img_height) {
    if (left_node_ && right_node_) {
        auto node = left_node_->insert_impl(img_width, img_height);
        if (node) {
            return node;
        } else {
            return right_node_->insert_impl(img_width, img_height);
        }
    } else {
        if (is_image_)
            return nullptr;
        if (rc_.width < img_width || rc_.height < img_height)
            return nullptr;
        if (rc_.width == img_width && rc_.height == img_height) {
            is_image_ = true;
            return shared_from_this();
        }

        left_node_ = std::make_shared<make_shared_helper<atlas_node> >(0, 0);
        right_node_ = std::make_shared<make_shared_helper<atlas_node> >(0, 0);

        auto dw = rc_.width - img_width;
        auto dh = rc_.height - img_height;

        if (dw > dh) {
            left_node_->rc_ = {rc_.x, rc_.y, img_width, rc_.height};
            right_node_->rc_ = {rc_.x + img_width, rc_.y, dw, rc_.height};
        } else {
            left_node_->rc_ = {rc_.x, rc_.y, rc_.width, img_height};
            right_node_->rc_ = {rc_.x, rc_.y + img_height, rc_.width, dh};
        }
        return insert_impl(img_width, img_height);
    }
}

atlas_node::atlas_node(int width, int height) : left_node_(nullptr), right_node_(nullptr),
    rc_({0, 0, width, height}), is_image_(false) {}

} // end of namespace hsu
