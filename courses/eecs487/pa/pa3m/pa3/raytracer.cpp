#include "raytracer.h"

#include <iostream>
#include <cstdlib>
#include <typeinfo>

using namespace std;

/// An auxilliary function that produces a pseudo-random floating
/// point number between 0 and 1
float frand() {
  return float(rand()) / (RAND_MAX);
}

void RayTracerT::TraceAll(const CameraT& cam, XImageT<XVec3b>& image) {

  // YOUR CODE HERE
  // viewing rays generation setup here
	double t = tan(cam.m_fovy/2) * cam.m_wdist;
	double r = t * cam.m_nx / cam.m_ny;
	double l = -r;
	double b = -t;

	double ws = - cam.m_wdist;

	vector<vector<XVec3f> > screen_points(cam.m_nx);

	for (int xi = 0; xi < cam.m_nx; xi++) { // x index
		screen_points[xi] = vector<XVec3f>(cam.m_nx);
		for (int yi = 0; yi < cam.m_ny; yi++) { // y index
			double us = l + (r - l) * (xi + 0.5) / cam.m_nx;
			double vs = b + (t - b) * (yi + 0.5) / cam.m_ny;
			screen_points[xi][yi] = cam.m_eyepos + us * cam.m_eyeu + vs * cam.m_eyev + ws * cam.m_eyew;
		}
	}

  // YOUR CODE HERE
  // antialiasing and area light setup goes here
	int N = m_opts.m_aasample * m_opts.m_aasample;
	double sub_side = 1.0 / m_opts.m_aasample;
	vector<vector<vector<vector<XVec3f> > > > screen_points_aa(cam.m_nx);
	for (int i = 0; i < cam.m_nx; i++) {
		screen_points_aa[i].resize(cam.m_ny);
		for (int j = 0; j < cam.m_ny; j++) {
			screen_points_aa[i][j].resize(m_opts.m_aasample);

			// Jittered screen points
			for (int si = 0; si < m_opts.m_aasample; si++) { // sub i
				screen_points_aa[i][j][si].resize(m_opts.m_aasample);
				for (int sj = 0; sj < m_opts.m_aasample; sj++) {
					double us = l + (r - l) * (i + (si + frand()) * sub_side) / cam.m_nx;
					double vs = b + (t - b) * (j + (sj + frand()) * sub_side) / cam.m_ny;
					screen_points_aa[i][j][si][sj] = cam.m_eyepos + us * cam.m_eyeu + vs * cam.m_eyev + ws * cam.m_eyew;
				}
			}

			//Shuffle
			for (int n = N-1; n > 0; n--) {
				int si = n % m_opts.m_aasample;
				int sj = n / m_opts.m_aasample;
				int n_rand = rand() % n;
				int si_rand = n_rand % m_opts.m_aasample;
				int sj_rand = n_rand / m_opts.m_aasample;
				swap(screen_points_aa[i][j][si][sj], screen_points_aa[i][j][si_rand][sj_rand]);
			}
		}
	}


  /// The main loop over all the pixels in the output image
  for(int i=0; i<cam.m_nx; ++i) {
    for(int j=0; j<cam.m_ny; ++j) {
      // generate a ray from the eye
      XVec3f color(0.0f);
						stack<float> ris;
						ris.push(1.0f);
      if(m_opts.m_aasample<=0) {
        // YOUR CODE HERE
        // viewing ray generation code replacing this default wrong ray
        ray_t vray(cam.m_eyepos, screen_points[i][j] - cam.m_eyepos);
        
        m_current_sample = 0;
       
        color = Trace(vray, 0, ris);
      } else {
        // YOUR CODE HERE
        // antialiasing and area light setup
				for (int si = 0; si < m_opts.m_aasample; si++) {
					for (int sj = 0; sj < m_opts.m_aasample; sj++) {
						ray_t vray(cam.m_eyepos, screen_points_aa[i][j][si][sj] - cam.m_eyepos);

						color += Trace(vray, 0, ris);
					}
				}

				color /= N;
      }
      // image pixel is assigned here -- simple clamping performed
      image.at(i,j) = XVec3b((unsigned char)(0xFF*std::min(1.0f, color(0))), 
                            (unsigned char)(0xFF*std::min(1.0f, color(1))), 
                            (unsigned char)(0xFF*std::min(1.0f, color(2))));
    }
  }
}

XVec3f diffuse(const hitinfo_t & hit, ILight * light, const SceneT & scene)
{
	vector<XVec3f> samplePos;
	if (typeid(*light) == typeid(PointLightT)) {
			PointLightT * al = dynamic_cast<PointLightT*>(light);
			samplePos.push_back(al->SamplePos());
	} else if (typeid(*light) == typeid(AreaLightT)) {
		AreaLightT * al = dynamic_cast<AreaLightT*>(light);
		samplePos = al->jitterPos(NUM_SAMPLE_AREA_LIGHT);
	}

	XVec3f color(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < samplePos.size(); i++) {
		XVec3f l = samplePos[i] - hit.m_pos;
		double dist = l.normalize();
		l = l / dist;
		ray_t shadow_ray = ray_t(hit.m_pos, l);
		hitinfo_t shadow_hit;
		float hit_alpha = scene.alpha_intersect(shadow_ray, shadow_hit, dist);
		//if (!scene.Intersect(shadow_ray, shadow_hit) || shadow_hit.m_t > 1)
			//color += light->Color();
		if (fabs(hit_alpha) > EPSILON)
			color += light->Color() * hit_alpha;
	}
	color /= samplePos.size();

	return color;
}

XVec3f refract(const XVec3f & in_dir, const XVec3f & norm, float n1, float n2)
{
	XVec3f ri = in_dir;
	ri /= ri.normalize();
	XVec3f n = norm;
	n /= n.normalize();
	float kernel = 1 - n1 * n1 / (n2 * n2) * (1 - n.dot(ri) * n.dot(ri));
	XVec3f rt = ri * n1 / n2 - n * (n.dot(ri) * n1 / n2 +
				 sqrt(kernel));

	return rt;
}

/// Returns the color from the shading computation using
/// the information in the hitinfo_t structure
/// level is the recursion level
XVec3f RayTracerT::Shade(const hitinfo_t& hit, int level, stack<float> ris) {
  XVec3f color(0.0f);

  // Ambient light contribution
  color = hit.m_mat.m_ca*hit.m_mat.m_cr;

  // YOUR CODE HERE
  // shading code here 
  // iterate over the lights and collect their contribution
  // make a recursive call to Trace() function to get the reflections
		XVec3f v = -hit.m_indir;
		v = v / v.normalize();
		XVec3f n = hit.m_normal;
		n = n / n.normalize();
	for (SceneT::LightCt::const_iterator it = m_scene.BeginLights(); it < m_scene.EndLights(); it++) {
		XVec3f l = (*it)->SamplePos() - hit.m_pos;
		l = l / l.normalize();
		XVec3f h = (l + v);
		h = h / h.normalize();

		XVec3f diff_color = diffuse(hit, (*it), m_scene);
		
		color += diff_color * (hit.m_mat.m_cr * max(0.0f, n.dot(l)) +
						 hit.m_mat.m_cp * pow(fabs(n.dot(h)), hit.m_mat.m_p));
	}
	XVec3f refl_dir;
	if (n.dot(v) > 0)
		refl_dir = 2 * n * n.dot(v) - v;
	else
		refl_dir = 2 * (-n) * (-n.dot(v)) - v;
	XVec3f refl_color = Trace(ray_t(hit.m_pos, refl_dir), level+1, ris); // Reflected color
	color += refl_color * hit.m_mat.m_cp;

	if (fabs(hit.m_mat.m_alpha - 1) > EPSILON) {
		float n1, n2;
		if (n.dot(v) > 0) {
			n1 = ris.top(); 
			n2 = hit.m_mat.m_ri;
			ris.push(hit.m_mat.m_ri);
		} else {
			n1 = ris.top();
			if (ris.size() == 1)
				return color;
			n2 = ris.top();
			n = -n;
		}
		XVec3f refr_vec = refract(hit.m_indir, n, n1, n2);
		if (refr_vec[0] != NAN) {
			ray_t refr_ray = ray_t(hit.m_pos, refr_vec);
			XVec3f refr_color = Trace(refr_ray, level+1, ris);

			color = color * hit.m_mat.m_alpha + (1 - hit.m_mat.m_alpha) * refr_color;
		}
	}

  return color;
}


/// Returns the color value coming along the ray
/// level is the recursion level
XVec3f RayTracerT::Trace(const ray_t& ray, int level, stack<float> ris) {
  // YOUR CODE HERE
  // limit the recursion level here

	if (level >= m_opts.m_max_recursion) {
		return m_scene.BackgroundColor();
	}

  XVec3f color;

  hitinfo_t hit;

  if(!m_scene.Intersect(ray, hit)) {
		return m_scene.BackgroundColor();
	}

  color = Shade(hit, level, ris);

  return color;
}

