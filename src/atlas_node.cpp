//
//  atlas_node.cpp
//  atlas-tree-xcode
//
//  Created by Hsu Chaohung on 2017/06/08.
//  Copyright © 2017年 hsu. All rights reserved.
//

#include "atlas_node.hpp"

namespace hsu {

atlas_node::atlas_node(int width, int height) : left_node_(), right_node_(),
    rc_({0, 0, width, height}), is_image_(false) {}

std::shared_ptr<atlas_node const> atlas_node::insert(image const& img) {
    if (left_node_ && right_node_) {
        auto node = left_node_->insert(img);
        if (node) {
            return node;
        } else {
            return right_node_->insert(img);
        }
    } else {
        if (is_image_)
            return nullptr;
        if (rc_.width < img.width || rc_.height < img.height)
            return nullptr;
        if (rc_.width == img.width && rc_.height == img.height) {
            is_image_ = true;
            return shared_from_this();
        }

        left_node_ = std::make_shared<atlas_node>(0, 0);
        right_node_ = std::make_shared<atlas_node>(0, 0);

        auto dw = rc_.width - img.width;
        auto dh = rc_.height - img.height;

        if (dw > dh) {
            left_node_->rc_ = {rc_.x, rc_.y, img.width, rc_.height};
            right_node_->rc_ = {rc_.x + img.width, rc_.y, dw, rc_.height};
        } else {
            left_node_->rc_ = {rc_.x, rc_.y, rc_.width, img.height};
            right_node_->rc_ = {rc_.x, rc_.y + img.height, rc_.width, dh};
        }
        return insert(img);
    }
}

} // end of namespace hsu
