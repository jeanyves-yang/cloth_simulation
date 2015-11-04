/*
**    TP CPE Lyon
**    Copyright (C) 2015 Damien Rohmer
**
**    This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**   This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#ifndef MESH_PARAMETRIC_CLOTH_HPP
#define MESH_PARAMETRIC_CLOTH_HPP

#include "../lib/mesh/mesh_parametric.hpp"
#include "../lib/common/exception_cpe.hpp"
#include "../lib/mesh/mesh.hpp"

namespace cpe
{
class mesh_parametric_cloth : public mesh_parametric
{
public:
    using mesh_parametric::mesh_parametric;

    void set_plane_xy_unit(int const size_u_param,int const size_v_param);
    void get_mesh_cat(const mesh &cat_mesh);

    vec3 const& speed(int ku,int kv) const;
    vec3& speed(int ku,int kv);

    vec3 const& force(int ku,int kv) const;
    vec3& force(int ku,int kv);

    void update_force();
    void integration_step(float dt);

    int K1 = 50;
    int K2 = 10;
    float K3 = 5;
    float K_wind = 0.008f;

    float u_wind_x=0.0f;
    float u_wind_y=0.0f;
    float u_wind_z=0.0f;


    int rayon_sphere = 2;
    bool is_chat;
    bool is_cube;
    bool is_sphere;


private:

    std::vector<vec3> speed_data;
    std::vector<vec3> force_data;
    /** Mesh of a cat */
    cpe::mesh mesh_cat;

};

class exception_divergence : public exception_cpe
{
    using exception_cpe::exception_cpe;
};

}

#endif
