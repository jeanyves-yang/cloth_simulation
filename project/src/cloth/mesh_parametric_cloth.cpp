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

#include "mesh_parametric_cloth.hpp"

#include "../lib/common/error_handling.hpp"
#include <cmath>
#include <list>

namespace cpe
{

void mesh_parametric_cloth::get_mesh_cat(const mesh& cat_mesh)
{
    mesh_cat = cat_mesh;
}

void mesh_parametric_cloth::update_force()
{

    int const Nu = size_u();
    int const Nv = size_v();
    int const N_total = Nu*Nv;
    ASSERT_CPE(static_cast<int>(force_data.size()) == Nu*Nv , "Error of size");


    //*************************************************************//
    // TO DO, Calculer les forces s'appliquant sur chaque sommet
    //*************************************************************//

    //*************************************************************//

    //Gravity
    static vec3 const g (0.0f,0.0f,-9.81f);
    vec3 const g_normalized = g/N_total;
    float L_rest = 1.0f/(float(Nu)-1.0f);
    vec3 wind(0.0f,0.0f,0.0f);
    wind.x() = 0.0f;
    wind.y() = 0.0f;
    wind.z() = 0.0f;

    wind.x() = u_wind_x-3;
    wind.x() = wind.x()/6.0f;
    wind.y() = u_wind_y-3;
    wind.y() = wind.y()/6.0f;

    /* Structural springs : en (i,j) possibilité de voisins en (i+1,j), (i-1,j), (i,j+1), (i,j-1)
       Shearing springs : en (i,j) possibilité de voisins en (i+1,j+1), (i+1,j-1), (i-1,j-1), (i-1,j+1)
       Bending springs : en (i,j) possibilité de voisins en (i+2,j), (i-2,j), (i,j+2), (i,j-2)*/

    for(int ku=0 ; ku<Nu ; ++ku)
    {
        for(int kv=0 ; kv<Nv ; ++kv)
        {
            vec3 const& p_courant = vertex(ku,kv); // Recuperation du point courant
            force(ku,kv) = vec3(0,0,0);

            std::list<vec3> liste_voisins_struct;
            std::list<vec3> liste_voisins_shear;
            std::list<vec3> liste_voisins_bend;

            // ****************************************************************************//
            // ***************************STRUCTURAL SPRINGS*******************************//
            // ****************************************************************************//
            if(ku-1 >= 0)
            {
                vec3 const& voisin_gauche_struct = vertex(ku-1,kv); // Definition des voisins
                liste_voisins_struct.push_back(voisin_gauche_struct);
            }
            if(ku+1 <= Nu-1)
            {
                vec3 const& voisin_droite_struct = vertex(ku+1,kv);
                liste_voisins_struct.push_back(voisin_droite_struct);
            }
            if(kv-1 >= 0)
            {
                vec3 const& voisin_haut_struct = vertex(ku,kv-1);
                liste_voisins_struct.push_back(voisin_haut_struct);
            }
            if(kv+1 <= Nv-1)
            {
                vec3 const& voisin_bas_struct = vertex(ku,kv+1);
                liste_voisins_struct.push_back(voisin_bas_struct);
            }

            for (std::list<vec3>::iterator it = liste_voisins_struct.begin(); it != liste_voisins_struct.end(); it++)
            {
                vec3 const u = p_courant - *it;
                float const L = norm(u);
                force(ku,kv) += K1 * (L_rest-L) * u/L;
            }


            //***************************************************************//
            //********************** SHEARING SPRINGS ***********************//
            //***************************************************************//
            if(ku-1 >= 0 && kv-1 >= 0)
            {
                vec3 const& voisin_gh_shear = vertex(ku-1,kv-1); // Definition des voisins
                liste_voisins_shear.push_back(voisin_gh_shear);
            }
            if(ku+1 <= Nu-1 && kv-1 >= 0)
            {
                vec3 const& voisin_dh_shear = vertex(ku+1,kv-1);
                liste_voisins_shear.push_back(voisin_dh_shear);
            }
            if(ku-1 >= 0 && kv+1 <= Nv-1)
            {
                vec3 const& voisin_gb_struct = vertex(ku-1,kv+1);
                liste_voisins_shear.push_back(voisin_gb_struct);
            }
            if(ku+1 <= Nu-1 && kv+1 <= Nv-1)
            {
                vec3 const& voisin_db_struct = vertex(ku+1,kv+1);
                liste_voisins_shear.push_back(voisin_db_struct);
            }

            for (std::list<vec3>::iterator it = liste_voisins_shear.begin(); it != liste_voisins_shear.end(); it++)
            {
                vec3 const u = p_courant - *it;
                float const L = norm(u);
                force(ku,kv) += K2 * (sqrt(2*L_rest*L_rest)-L) * u/L;
            }

            //************************************************************************//
            //****************************BENDING SPRINGS*****************************//
           //************************************************************************//
            if(ku-2 >= 0)
            {
                vec3 const& voisin_gauche_bend = vertex(ku-2,kv); // Definition des voisins
                liste_voisins_bend.push_back(voisin_gauche_bend);
            }
            if(ku+2 <= Nu-1)
            {
                vec3 const& voisin_droite_bend = vertex(ku+2,kv);
                liste_voisins_bend.push_back(voisin_droite_bend);
            }
            if(kv-2 >= 0)
            {
                vec3 const& voisin_haut_bend = vertex(ku,kv-2);
                liste_voisins_bend.push_back(voisin_haut_bend);
            }
            if(kv+2 <= Nv-1)
            {
                vec3 const& voisin_bas_bend = vertex(ku,kv+2);
                liste_voisins_bend.push_back(voisin_bas_bend);
            }
            for (std::list<vec3>::iterator it = liste_voisins_bend.begin(); it != liste_voisins_bend.end(); it++)
            {
                vec3 const u = p_courant - *it;
                float const L = norm(u);
                force(ku,kv) += K3 * (2*L_rest-L) * u/L;
            }
            if( ku+1<Nu )
            {
                vec3 normale = normalized(normal(ku,kv));
                vec3 force_vent = K_wind * dot(normale, wind) * normale;
                force(ku,kv) += force_vent;

            }

            if(ku-1 >= 0)
            {
                vec3 normale = normalized(normal(ku,kv));
                vec3 force_vent = K_wind * dot(normale, wind) * normale;
                force(ku,kv) += force_vent;
            }

            if(kv-1 >= 0)
            {
                vec3 normale = normalized(normal(ku,kv));
                vec3 force_vent = K_wind * dot(normale, wind) * normale;
                force(ku,kv) += force_vent;
            }
            if(kv+1 <= Nv-1)
            {
                vec3 normale = normalized(normal(ku,kv));
                vec3 force_vent = K_wind * dot(normale, wind) * normale;
                force(ku,kv) += force_vent;
            }


            force(ku,kv) += g_normalized;

        }
    }
    force(0,0) = vec3(0.0f,0.0f,0.0f);
    force(Nu-1,0) = vec3(0.0f,0.0f,0.0f);



}

void mesh_parametric_cloth::integration_step(float const dt)
{
    ASSERT_CPE(speed_data.size() == force_data.size(),"Incorrect size");
    ASSERT_CPE(static_cast<int>(speed_data.size()) == size_vertex(),"Incorrect size");


    int const Nu = size_u();
    int const Nv = size_v();
    float const mu = 0.2f;

    //*************************************************************//
    // TO DO: Calculer l'integration numerique des positions au cours de l'intervalle de temps dt.
    //*************************************************************//
    for(int ku=0; ku<Nu; ++ku)
    {
        for(int kv=0; kv<Nv; ++kv)
        {
            if(vertex(ku,kv).z() <= -1.10f)
            {
                vertex(ku,kv).z() = -1.09f;
                speed(ku,kv).z() = 0.0f;
                force(ku,kv).z() = 0.0f;
            }
//            if(vertex(ku,kv).x() > 0.3f && vertex(ku,kv).y() > -0.15f && vertex(ku,kv).y() < 0.25f && vertex(ku,kv).z() >-1.3f && vertex(ku,kv).z() < -0.9f)
//            {
//                std::cout<<"collision avec le cube"<<std::endl;
//            }

//            if(vertex(ku,kv).x() < 0.7f && vertex(ku,kv).y() > -0.15f && vertex(ku,kv).y() < 0.25f && vertex(ku,kv).z() >-1.3f && vertex(ku,kv).z() < -0.9f)
//            {
//                std::cout<<"collision avec le cube"<<std::endl;
//            }

//            if(vertex(ku,kv).z() < -0.9f && vertex(ku,kv).x() > 0.3f && vertex(ku,kv).x() < 0.7f && vertex(ku,kv).y() > -0.15f && vertex(ku,kv).y() < 0.25f)
//            {
//                std::cout<<"collision avec le cube"<<std::endl;
//            }
//            if(vertex(ku,kv).y() < 0.25f && vertex(ku,kv).x() > 0.3f && vertex(ku,kv).x() < 0.7f && vertex(ku,kv).z() >-1.3f && vertex(ku,kv).z() < -0.9f)
//            {
//                std::cout<<"collision avec le cube"<<std::endl;
//            }
//            if(vertex(ku,kv).y() > -0.15f && vertex(ku,kv).x() > 0.3f && vertex(ku,kv).x() < 0.7f && vertex(ku,kv).z() >-1.3f && vertex(ku,kv).z() < -0.9f)
//            {
//                std::cout<<"collision avec le cube"<<std::endl;
//            }


            if(is_sphere == true)
            {
                vec3 const d = vertex(ku,kv) - vec3(0.5f,0.05f,-1.1f);
                float const distance_sphere = norm(d);

                float rayon = 0.198*rayon_sphere/2 + (0.012*rayon_sphere);
                if(distance_sphere <= rayon)
                {
                    // decomposition de la vitesse
                    vec3 normal = d/distance_sphere;
                    speed(ku,kv) -= dot(speed(ku,kv),normal) * normal;
                    //projection sur la sphere (centre sphere + rayon sphere * normale)
                    vertex(ku,kv) = vec3(0.5f,0.05f,-1.1f) + (rayon+0.01) * normal;
                    force(ku,kv) -= dot(force(ku,kv),normal) * normal;
                }
                else
                {
                    speed(ku,kv) = (1-mu*dt)*speed(ku,kv) + dt*force(ku,kv);
                    vertex(ku,kv) = vertex(ku,kv) +dt*speed(ku,kv);
                }
            }
            if(is_chat == true)
            {
                for(int k = 0; k < mesh_cat.size_vertex(); k++)
                {
                    vec3 const d_cat = vertex(ku,kv) - mesh_cat.vertex(k);
                    float const distance_cat = norm(d_cat);

                    float rayon_sphere = 0.03f;
                    if(distance_cat <= rayon_sphere)
                    {
                        // decomposition de la vitesse
                        vec3 normal = d_cat/distance_cat;
                        force(ku,kv) -= dot(force(ku,kv),normal) * normal;
                        speed(ku,kv) -= dot(speed(ku,kv),normal) * normal;
                        //projection sur la sphere (centre sphere + rayon sphere * normale)
                        vertex(ku,kv) = mesh_cat.vertex(k) + (rayon_sphere) * normal;

                    }
                }
                    //else
                    //{
                speed(ku,kv) = (1-mu*dt)*speed(ku,kv) + dt*force(ku,kv);
                vertex(ku,kv) = vertex(ku,kv) +dt*speed(ku,kv);
                    //}
            }

        }
    }
    //*************************************************************//


    //security check (throw exception if divergence is detected)
    static float const LIMIT=30.0f;
    for(int ku=0 ; ku<Nu ; ++ku)
    {
        for(int kv=0 ; kv<Nv ; ++kv)
        {
            vec3 const& p = vertex(ku,kv);

            if( norm(p) > LIMIT )
            {
                throw exception_divergence("Divergence of the system",EXCEPTION_PARAMETERS_CPE);
            }
        }
    }

}

void mesh_parametric_cloth::set_plane_xy_unit(int const size_u_param,int const size_v_param)
{
    mesh_parametric::set_plane_xy_unit(size_u_param,size_v_param);

    int const N = size_u()*size_v();
    speed_data.resize(N);
    force_data.resize(N);
}

vec3 const& mesh_parametric_cloth::speed(int const ku,int const kv) const
{
    ASSERT_CPE(ku >= 0 , "Value ku ("+std::to_string(ku)+") should be >=0 ");
    ASSERT_CPE(ku < size_u() , "Value ku ("+std::to_string(ku)+") should be < size_u ("+std::to_string(size_u())+")");
    ASSERT_CPE(kv >= 0 , "Value kv ("+std::to_string(kv)+") should be >=0 ");
    ASSERT_CPE(kv < size_v() , "Value kv ("+std::to_string(kv)+") should be < size_v ("+std::to_string(size_v())+")");

    int const offset = ku + size_u()*kv;

    ASSERT_CPE(offset < static_cast<int>(speed_data.size()),"Internal error");

    return speed_data[offset];
}

vec3& mesh_parametric_cloth::speed(int const ku,int const kv)
{
    ASSERT_CPE(ku >= 0 , "Value ku ("+std::to_string(ku)+") should be >=0 ");
    ASSERT_CPE(ku < size_u() , "Value ku ("+std::to_string(ku)+") should be < size_u ("+std::to_string(size_u())+")");
    ASSERT_CPE(kv >= 0 , "Value kv ("+std::to_string(kv)+") should be >=0 ");
    ASSERT_CPE(kv < size_v() , "Value kv ("+std::to_string(kv)+") should be < size_v ("+std::to_string(size_v())+")");

    int const offset = ku + size_u()*kv;

    ASSERT_CPE(offset < static_cast<int>(speed_data.size()),"Internal error");

    return speed_data[offset];
}

vec3 const& mesh_parametric_cloth::force(int const ku,int const kv) const
{
    ASSERT_CPE(ku >= 0 , "Value ku ("+std::to_string(ku)+") should be >=0 ");
    ASSERT_CPE(ku < size_u() , "Value ku ("+std::to_string(ku)+") should be < size_u ("+std::to_string(size_u())+")");
    ASSERT_CPE(kv >= 0 , "Value kv ("+std::to_string(kv)+") should be >=0 ");
    ASSERT_CPE(kv < size_v() , "Value kv ("+std::to_string(kv)+") should be < size_v ("+std::to_string(size_v())+")");

    int const offset = ku + size_u()*kv;

    ASSERT_CPE(offset < static_cast<int>(force_data.size()),"Internal error");

    return force_data[offset];
}

vec3& mesh_parametric_cloth::force(int const ku,int const kv)
{
    ASSERT_CPE(ku >= 0 , "Value ku ("+std::to_string(ku)+") should be >=0 ");
    ASSERT_CPE(ku < size_u() , "Value ku ("+std::to_string(ku)+") should be < size_u ("+std::to_string(size_u())+")");
    ASSERT_CPE(kv >= 0 , "Value kv ("+std::to_string(kv)+") should be >=0 ");
    ASSERT_CPE(kv < size_v() , "Value kv ("+std::to_string(kv)+") should be < size_v ("+std::to_string(size_v())+")");

    int const offset = ku + size_u()*kv;

    ASSERT_CPE(offset < static_cast<int>(force_data.size()),"Internal error");

    return force_data[offset];
}


}
