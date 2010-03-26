/*
 * Name: 			Fu Yu
 * UMID: 			33551750
 * uniqname:	yufu
 */

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
			if (hit.m_pos.z() > _b + EPSILON && hit.m_pos.z() < _t-EPSILON)
				hit.m_normal = XVec3f(hit.m_pos.x(), hit.m_pos.y(), 0.0f);
			else
				hit.m_normal = XVec3f(0.0f, 0.0f, -ray.m_dir.z());

			hit.m_mat = m_mat;

			/// transform it back to the world coordinates
			hit.m_pos = m_xf.apply(hit.m_pos);
			hit.m_normal = m_xfi.applytv(hit.m_normal);
			hit.m_normal.normalize();

			return true;
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
