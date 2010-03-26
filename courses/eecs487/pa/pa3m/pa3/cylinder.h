#ifndef _CYLINDER_H
#define _CYLINDER_H

#include <algorithm>
#include <iostream>
#include "constants.h"

using namespace std;

class CylinderT: public IGel {
	public:
		CylinderT() { _init(); }
		CylinderT(const XFormf & _xf) : m_xf(_xf)
		{
			m_xfi = m_xf.Inverse();
			_init();
		}

		virtual ~CylinderT() {}
		
		virtual bool Intersect(const ray_t& wray, hitinfo_t & hit) const {

			// Take care of the transform
			ray_t ray(m_xfi.apply(wray.m_origin), m_xfi.applyv(wray.m_dir));

			float A = ray.m_dir.x() * ray.m_dir.x() + ray.m_dir.y() * ray.m_dir.y();
			float B = 2 * (ray.m_origin.x() * ray.m_dir.x() + ray.m_origin.y() *
								ray.m_dir.y());
			float C = ray.m_origin.x() * ray.m_origin.x() +
								ray.m_origin.y() * ray.m_origin.y() - _r * _r;
			float det = B * B - 4 * A * C;
			
			if (det <= 0)
				return false;

			float ts1, ts2; // Side
			float tz1, tz2; // z axis (top and bottom)

			if (fabs(ray.m_dir.z()) < EPSILON) {
				// parallel to the top and bottom
				if (ray.m_origin.z() < _b || ray.m_origin.z() > _t)
					return false;
				else {
					tz1 = -INFINITY;
					tz2 = INFINITY;
				} 
			} else {
				tz1 = (_b - ray.m_origin.z()) / ray.m_dir.z();
				tz2 = (_t - ray.m_origin.z()) / ray.m_dir.z();
			}

			if (tz2 < tz1) { float tmp = tz1; tz1 = tz2; tz2 = tmp;}
			
			if (fabs(A) < EPSILON) 
				if (C > EPSILON) return false;
				else if (C < -EPSILON) {
					ts1 = -INFINITY;
					ts2 = INFINITY;
				} else
					ts1 = ts2 = - C / B; // Will there be reflection?
			else {
				ts1 = 0.5f * (-B - sqrt(det)) / A;
				ts2 = 0.5f * (-B + sqrt(det)) / A;
			}

			if (ts2 < tz1 || tz2 < ts1 || (ts2 < 0 && tz2 < 0)) return false;
			float ta[] = {ts1, ts2, tz1, tz2};
			float *t = ta;
			sort(t, t+4);
			while (*(++t) < -EPSILON);
			if (t - ta == 3) return false;

			//cout << "t: " << *t << " Index: " << t - ta << " more: " << ts1 << " " << ts2 << " " << tz1 << " " << tz2 << endl;
			//cout << "A: " << A << " B: " << B << " C: " << C << endl;
			//cout << "ray origin: " << ray.m_origin << endl;
			assert(t - ta < 4);

			hit.m_t = *t;
			hit.m_pos = ray.at(*t);
		
			// What if hitting the edge?
			if (hit.m_pos.z() > EPSILON && hit.m_pos.z() < 1-EPSILON)
				hit.m_normal = XVec3f(hit.m_pos.x(), hit.m_pos.y(), 0.0f);
			else
				hit.m_normal = XVec3f(0.0f, 0.0f, -ray.m_dir.z());

			hit.m_mat = m_mat;

			/// transform it back to the world coordinates
			hit.m_pos = m_xf.apply(hit.m_pos);
			hit.m_normal = m_xfi.applytv(hit.m_normal);
			hit.m_normal.normalize();

			return true;

        /*ray_t ray(m_xfi.apply(wray.m_origin), m_xfi.applyv(wray.m_dir));
       
        //a,b,c are parameters for an infinite cylinder
        float a = ray.m_dir(0) * ray.m_dir(0) + ray.m_dir(1) * ray.m_dir(1);
        float b = 2*( ray.m_dir(0)*ray.m_origin(0) + ray.m_dir(1)*ray.m_origin(1) );
        float c = ray.m_origin(0) * ray.m_origin(0) + ray.m_origin(1) * ray.m_origin(1)
        - m_radius * m_radius;
        float det = b*b - 4*a*c;
       
        //t, for the infinite cylinder
        float t1 = 0.5f*( -b - sqrt(det) )/a;
        float t2 = 0.5f*( -b + sqrt(det) )/a;
        //normal of top plane
        XVec3f n_top(0, 0, 1);
        //normal of bottom plane
        XVec3f n_bot(0, 0, -1);
        //point on top plane
        XVec3f p_top(0, 0, m_top);
        //point on bottom plane
        XVec3f p_bot(0, 0, m_bot);
        //t, for the top plane
        float t_tout = n_top.dot((p_top - ray.m_origin)) / n_top.dot(ray.m_dir); //outward
        float t_tin = n_top.dot((p_bot - ray.m_origin)) / n_top.dot(ray.m_dir); //inward
        //t, for the bottom plane
        float t_bout = n_bot.dot((p_bot - ray.m_origin)) / n_bot.dot(ray.m_dir); //outward
        float t_bin = n_bot.dot((p_top - ray.m_origin)) / n_bot.dot(ray.m_dir); //inward
       
        float top_bound = m_top + 1.0e-5;
        float bot_bound = m_bot - 1.0e-5;
       
        //points of ray-infinite cylinter intersection
        XVec3f v1 = ray.at(t1);
        XVec3f v2 = ray.at(t2);
       
        //if parallel to +z axis
        if (ray.m_dir.dot(n_top) == 1 && c <= 0) {
            if (t_bout > 0) {
                hit.m_t = t_bout;
                hit.m_pos = ray.at(t_bout);
                hit.m_normal = n_bot;
            } else if (t_tin > 0) {
                hit.m_t = t_tin;
                hit.m_pos = ray.at(t_tin);
                hit.m_normal = n_bot;
            } else {
                return false;
            }
        } else if (ray.m_dir.dot(n_bot) == 1 && c <= 0) { //if parallel to -z axis
            if (t_tout > 0) {
                hit.m_t = t_tout;
                hit.m_pos = ray.at(t_tout);
                hit.m_normal = n_top;
            } else if (t_tin > 0) {
                hit.m_t = t_bin;
                hit.m_pos = ray.at(t_bin);
                hit.m_normal = n_top;
            } else {
                return false;
            }
        } else if (det <= 0.0f) {
            return false;
        }
       
        if (t1 > 0) {
            //if ray means looking into planar x-y, then v1.z must between top & bot
            if (ray.m_dir.dot(n_top) >= 0) {
                //if ray means looking up into cylinder
                //then v1.z below top and v2.z abv bot
                if (v1(2) <= top_bound && v2(2) >= bot_bound) {
                    //if v1.z below bot then calculate intersection with bot-plane
                    if (v1(2) <= bot_bound) {
                        hit.m_t = t_bout;
                        hit.m_pos = ray.at(t_bout);
                        hit.m_normal = n_bot;
                    } else {
                        hit.m_t = t1;
                        hit.m_pos = v1;
                        hit.m_normal = XVec3f(hit.m_pos(0), hit.m_pos(1), 0.0f);
                    }
                } else {
                    return false;
                }
            } else {
                //if ray means looking down into cylinder
                //then v1.z abv bot and v2.z below top
                if (v1(2) >= bot_bound && v2(2) <= top_bound) {
                    //if v1.z above top plane
                    if (v1(2) >= top_bound) {
                        hit.m_t = t_tout;
                        hit.m_pos = ray.at(t_tout);
                        hit.m_normal = n_top;
                    } else {
                        hit.m_t = t1;
                        hit.m_pos = v1;
                        hit.m_normal = XVec3f(hit.m_pos(0), hit.m_pos(1), 0.0f);
                    }
                } else {
                    return false;
                }
            }
        } else if (t2 > 0) {
            //looking down
            if (ray.m_dir.dot(n_top) <= 0) {
                if (ray.m_origin(2) >= top_bound && v2(2) <= top_bound) {
                    //if eye abv top and t2.z below top
                    hit.m_t = t_tout;
                    hit.m_pos = ray.at(t_tout);
                    hit.m_normal = n_top;
                } else if (ray.m_origin(2) >= bot_bound) {
                    //if eye abv bot
                    if (v2(2) <= bot_bound) {
                        hit.m_t = t_bin;
                        hit.m_pos = ray.at(t_bin);
                        hit.m_normal = n_top;
                    } else {
                        hit.m_t = t2;
                        hit.m_pos = v2;
                        hit.m_normal = XVec3f(hit.m_pos(0), hit.m_pos(1), 0.0f);
                    }
                } else {
                    return false;
                }

            } else {
                if (ray.m_origin(2) <= bot_bound && v2(2) >= bot_bound) {
                    //if eye below bot and t2.z abv bot
                    hit.m_t = t_bout;
                    hit.m_pos = ray.at(t_bout);
                    hit.m_normal = n_bot;
                } else if (ray.m_origin(2) <= top_bound) {
                    //if eye below top
                    if (v2(2) >= top_bound) {
                        hit.m_t = t_tin;
                        hit.m_pos = ray.at(t_tin);
                        hit.m_normal = n_bot;
                    } else {
                        hit.m_t = t2;
                        hit.m_pos = v2;
                        hit.m_normal = XVec3f(hit.m_pos(0), hit.m_pos(1), 0.0f);
                    }
                } else {
                    return false;
                }

            }
           
        } else {
            return false;
        }
       
        hit.m_mat = m_mat;
        // transform it back to the world coordinates
        hit.m_pos = m_xf.apply(hit.m_pos);
        hit.m_normal = m_xfi.applytv(hit.m_normal);
        hit.m_normal.normalize();
       
        return true;*/
				
		}
		
	public:
		XFormf m_xf, m_xfi;
		MaterialT m_mat;

	private:
		float _r, _t, _b; // radius, top, bottom
		float m_radius, m_bot, m_top;

		void _init() {_r = 1.0f; _t = 1.0; _b = 0.0;
			m_radius = 1.0; m_bot = 0.0; m_top = 1.0;
		}
};

#endif
