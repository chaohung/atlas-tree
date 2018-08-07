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

std::shared_ptr<atlas_node const> atlas_node::insert(int img_width, int img_height, int margin) {
    img_width += margin * 2;
    img_height += margin * 2;
    auto img_node = insert_impl(img_width, img_height);

    if (!img_node || (rc_.width == img_width && rc_.height == img_height) ) {
        auto new_width = rc_.width + img_width;
        auto new_height = rc_.height + img_height;
        resize(new_width, new_height);
        return insert(img_width - margin * 2, img_height - margin * 2);
    }
    img_node->rc_.x += margin;
    img_node->rc_.y += margin;
    img_node->rc_.width -= margin * 2;
    img_node->rc_.height -= margin * 2;

    return img_node;
}

void atlas_node::remove(std::shared_ptr<atlas_node const> node) {
    auto raw_node = std::const_pointer_cast<atlas_node>(node);
    raw_node->is_image_ = false;

    while (auto parent_node = raw_node->parent_node_.lock()) {
        if (parent_node->left_node_->is_activated() ||
            parent_node->right_node_->is_activated()) break;
        parent_node->left_node_.reset();
        parent_node->right_node_.reset();
        raw_node = parent_node;
    }
}

void atlas_node::resize(int new_width, int new_height) {
    if (left_node_ && right_node_) {
        if (new_height <= rc_.width || new_height <= rc_.height) return;
    }

    auto tmp_rc = rc_;
    auto tmp_left_node = left_node_;
    auto tmp_right_node = right_node_;
    rc_ = {0, 0, new_width, new_height};
    left_node_.reset();
    right_node_.reset();

    if (tmp_rc.width > 0 && tmp_rc.height > 0) {
        auto node = insert_impl(tmp_rc.width, tmp_rc.height);
        if (node) {
            if (tmp_left_node)
                tmp_left_node->parent_node_ = node;
            if (tmp_right_node)
                tmp_right_node->parent_node_ = node;
            node->is_image_ = false;
            node->left_node_ = tmp_left_node;
            node->right_node_ = tmp_right_node;
        }
    }
    if (resize_handler) {
        resize_handler(tmp_rc.width, tmp_rc.height, new_width, new_height);
    }
}

void atlas_node::clear() {
    traverse([] (std::shared_ptr<atlas_node const> node) {
        auto raw_node = std::const_pointer_cast<atlas_node>(node);
        raw_node->is_image_ = false;
        raw_node->left_node_ = nullptr;
        raw_node->right_node_ = nullptr;
    });
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
        left_node_->parent_node_ = shared_from_this();
        right_node_ = std::make_shared<make_shared_helper<atlas_node> >(0, 0);
        right_node_->parent_node_ = shared_from_this();

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

atlas_node::atlas_node(int width, int height) :
    parent_node_(), left_node_(nullptr), right_node_(nullptr),
    rc_({0, 0, width, height}), is_image_(false) {}

} // end of namespace hsu
