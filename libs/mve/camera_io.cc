/*
 * Copyright (C) 2016, Samir Aroudj
 * TU Darmstadt - Graphics, Capture and Massively Parallel Computing
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the LICENSE.txt file for details.
 */

#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <fstream>
#include "mve/camera_io.h"
#include "util/file_system.h"
#include "util/strings.h"
#include "util/exception.h"

MVE_NAMESPACE_BEGIN

/* ---------------------------------------------------------------- */

void
save_camera_infos (const std::vector<View::Ptr> &views, std::string const& file_name)
{
    const size_t view_count = views.size();
    std::cout << "Writing camera infos of (" << view_count << " cameras: " << file_name << "...\n";

    std::ofstream out(file_name.c_str(), std::ios::binary);
    if (!out.good())
        throw util::FileException(file_name, std::strerror(errno));

    size_t actual_view_count = 0;
    for (size_t view_idx = 0; view_idx < view_count; ++view_idx)
        if (nullptr != views[view_idx])
            ++actual_view_count;

    out << "MVE camera infos 1.3\n";
    out << "camera_count = " << actual_view_count << "\n";
    out << "\n";

    /* Write all cameras infos to file. */
    for (size_t view_idx = 0; view_idx < view_count; ++view_idx)
    {
        if (nullptr == views[view_idx])
            continue;

        /* get view & camera & name*/
        View &view = *(views[view_idx]);
        CameraInfo const& cam = view.get_camera();
        std::string name = view.get_name();
        if (name.empty())
            name = std::to_string(view.get_id());

        /* get undistorted image resolution for image apsect ratio */
        float image_aspect = 1.0f;
        View::ImageProxy const* proxy = view.get_image_proxy("undistorted");
        if (proxy)
            image_aspect = (float) proxy->width / (float) proxy->height;

        /* intrinsics */
        out << "focal_length = " << cam.flen << "\n";
        out << "image_aspect = " << image_aspect << "\n";
        out << "pixel_aspect = " << cam.paspect << "\n";
        out << "principal_point = " << cam.ppoint[0] << " " << cam.ppoint[1] << "\n";
        out << "camera_distortion = " << cam.dist[0] << " " << cam.dist[1] << "\n";

        /* extrinsics */
        out << "rotation = ";
        out	<< cam.rot[0] << " " << cam.rot[1] << " " << cam.rot[2] << " ";
        out	<< cam.rot[3] << " " << cam.rot[4] << " " << cam.rot[5] << " ";
        out	<< cam.rot[6] << " " << cam.rot[7] << " " << cam.rot[8] << "\n";
        out << "translation = " << cam.trans[0] << " " << cam.trans[1] << " " << cam.trans[2] << "\n";
        out << "time_stamp = " << 0.0f << "\n";

        /* identifiers */
        out << "id = " << view.get_id() << "\n";
        out << "name = " << name << "\n";
        out << "\n";
    }
}

/* ---------------------------------------------------------------- */

MVE_NAMESPACE_END

