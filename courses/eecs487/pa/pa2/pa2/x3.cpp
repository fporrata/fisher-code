// X3 Scene graph classes implementation. Look for YOUR CODE HERE sections.
// Classes are given in alphabetical order in this file.

// NOTE: Constructors of node objects take a list of name-attribute string 
// pairs list to get initialized. It is being called by the X3D parser.

/* Name:		Fu Yu
   Uniqname:	yufu
   UMID:		33551750
*/

#include <sstream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "x3.h"

#define NUM_TRI_SIDES	3
#define NUM_QUAD_SIDES	4

using namespace std;

// Auxilliary function to eliminate commas in attribute lists.
static void erase_char(string& s, char ch) {
  for(int i = 0; i < (int)s.length(); ++i) {
    if(s[i]==ch)
      s[i] = ' ';
  }
}

// Add method adds children to the nodes (only those allowed unless
// this is a grouping node). For instance, the Appearance node only 
// allows children of Material kind to be added, and everything else
// is skipped.
void X3Appearance::Add(X3NodeType type, X3Node* node) {
  if(type==X3NODE_MATERIAL) {
    material_ = reinterpret_cast<X3Material*>(node);
  } else {
    X3Node::Add(type, node);
  }
}
void X3Appearance::Print(std::ostream& ost, int offset) const {
  ost << std::string(offset, ' ') << Name() << std::endl;
  if(material_)
    material_->Print(ost, offset+1);
}

X3Box::X3Box(const char **atts) : size_(2.0f, 2.0f, 2.0f) {
  if(atts==0)
    return;
  for(const char** ref = atts; *ref!=NULL; ++ref) {
    const char* name = *ref;
    ++ref;
    if(*ref==NULL) {
      break;
    }
    if(strcmp(name, "size")==0) {
      sscanf(*ref, "%f %f %f", &size_(0), &size_(1), &size_(2));
    }
  }
}

void X3Box::Render() const {
  glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f( 0.5f*size_(0),  0.5f*size_(1),  0.5f*size_(2));
    glVertex3f(-0.5f*size_(0),  0.5f*size_(1),  0.5f*size_(2));
    glVertex3f(-0.5f*size_(0), -0.5f*size_(1),  0.5f*size_(2));
    glVertex3f( 0.5f*size_(0), -0.5f*size_(1),  0.5f*size_(2));

    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f( 0.5f*size_(0), -0.5f*size_(1), -0.5f*size_(2));
    glVertex3f(-0.5f*size_(0), -0.5f*size_(1), -0.5f*size_(2));
    glVertex3f(-0.5f*size_(0),  0.5f*size_(1), -0.5f*size_(2));
    glVertex3f( 0.5f*size_(0),  0.5f*size_(1), -0.5f*size_(2));

    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f( 0.5f*size_(0),  0.5f*size_(1),  0.5f*size_(2));
    glVertex3f( 0.5f*size_(0), -0.5f*size_(1),  0.5f*size_(2));
    glVertex3f( 0.5f*size_(0), -0.5f*size_(1), -0.5f*size_(2));
    glVertex3f( 0.5f*size_(0),  0.5f*size_(1), -0.5f*size_(2));

    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f*size_(0),  0.5f*size_(1), -0.5f*size_(2));
    glVertex3f(-0.5f*size_(0), -0.5f*size_(1), -0.5f*size_(2));
    glVertex3f(-0.5f*size_(0), -0.5f*size_(1),  0.5f*size_(2));
    glVertex3f(-0.5f*size_(0),  0.5f*size_(1),  0.5f*size_(2));

    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f( 0.5f*size_(0),  0.5f*size_(1),  0.5f*size_(2));
    glVertex3f( 0.5f*size_(0),  0.5f*size_(1), -0.5f*size_(2));
    glVertex3f(-0.5f*size_(0),  0.5f*size_(1), -0.5f*size_(2));
    glVertex3f(-0.5f*size_(0),  0.5f*size_(1),  0.5f*size_(2));

    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-0.5f*size_(0), -0.5f*size_(1),  0.5f*size_(2));
    glVertex3f(-0.5f*size_(0), -0.5f*size_(1), -0.5f*size_(2));
    glVertex3f( 0.5f*size_(0), -0.5f*size_(1), -0.5f*size_(2));
    glVertex3f( 0.5f*size_(0), -0.5f*size_(1),  0.5f*size_(2));
  glEnd();
}


X3Cone::X3Cone(const char** atts) 
  : bottom_(true), side_(true),
    height_(2.0f), bottom_radius_(1.0f) 
{
  if(atts==0)
    return;
  for(const char** ref = atts; *ref!=NULL; ++ref) {
    const char* name = *ref;
    ++ref;
    if(*ref==NULL) {
      break;
    }
    if(strcmp(name, "height")==0) {
      sscanf(*ref, "%f", &height_);
    } else if(strcmp(name, "bottomRadius")==0) {
      sscanf(*ref, "%f", &bottom_radius_);
    } else if(strcmp(name, "bottom")==0) {
      if(strcmp(*ref, "false")==0)
        bottom_ = false;
      else if(strcmp(*ref, "true")==0)
        bottom_ = true;
    } else if(strcmp(name, "side")==0) {
      if(strcmp(*ref, "false")==0)
        side_ = false;
      else if(strcmp(*ref, "true")==0)
        side_ = true;
    }
  }
}

void X3Cone::Render() const {
  const int N = 10;
  const float step = 2.0f * M_PI / N;
  if(side_) {
    glBegin(GL_QUAD_STRIP);
    for(int k = 0; k < N+1; ++k) {
      glNormal3f(cos(k*step), bottom_radius_ / height_, sin(k*step));
      glVertex3f(bottom_radius_*cos(k*step), -0.5f*height_, bottom_radius_*sin(k*step));
      glVertex3f(0.0f, 0.5f*height_, 0.0f);
    }
    glEnd();
  }
  if(bottom_) {
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, -0.5f*height_, 0.0f);
    for(int k = 0; k < N+1; ++k) {
      glVertex3f(bottom_radius_*cos(k*step), -0.5f*height_, bottom_radius_*sin(k*step));
    }
    glEnd();
  }
}


X3Coordinate::X3Coordinate(const char** atts) {
  if(atts==0)
    return;
  for(const char** ref = atts; *ref!=NULL; ++ref) {
    const char* name = *ref;
    ++ref;
    if(*ref==NULL) {
      break;
    }
    string ref_copy(*ref);
    erase_char(ref_copy, ',');
    if(strcmp(name, "point")==0) {
      point_.clear();
      istringstream ist(ref_copy);
      while(ist.good()) {
        XVec3f xyz;
        ist >> xyz(0) >> xyz(1) >> xyz(2);
        if(!ist.fail()) {
          point_.push_back(xyz);
        }
      }
    }
  }
}

X3Cylinder::X3Cylinder(const char** atts) 
  : top_(true), bottom_(true), side_(true),
    height_(2.0f), radius_(1.0f) 
{
  if(atts==0)
    return;
  for(const char** ref = atts; *ref!=NULL; ++ref) {
    const char* name = *ref;
    ++ref;
    if(*ref==NULL) {
      break;
    }
    if(strcmp(name, "height")==0) {
      sscanf(*ref, "%f", &height_);
    } else if(strcmp(name, "radius")==0) {
      sscanf(*ref, "%f", &radius_);
    } else if(strcmp(name, "bottom")==0) {
      if(strcmp(*ref, "false")==0)
        bottom_ = false;
      else if(strcmp(*ref, "true")==0)
        bottom_ = true;
    } else if(strcmp(name, "top")==0) {
      if(strcmp(*ref, "false")==0)
        top_ = false;
      else if(strcmp(*ref, "true")==0)
        top_ = true;
    } else if(strcmp(name, "side")==0) {
      if(strcmp(*ref, "false")==0)
        side_ = false;
      else if(strcmp(*ref, "true")==0)
        side_ = true;
    }
  }
}
void X3Cylinder::Render() const {
  // YOUR CODE HERE: Modify this function to render a cylinder.
  // Make sure to use top_, side_, and bottom_ flags and to compute 
  // and send the normals properly.
	const int N = 20;
	const float step = 2.0f * M_PI / N;
	if (side_) {
		glBegin(GL_QUAD_STRIP);
		for (int k = 0; k < N+1; ++k) {
			glNormal3f(cos(k*step), 0, sin(k*step));
			glVertex3f(radius_ * cos(k*step), -height_*0.5, radius_*sin(k*step));
			glVertex3f(radius_ * cos(k*step), height_ * 0.5, radius_ * sin(k*step));
		}
		glEnd();
	}

	if (bottom_) {
		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f(0.0f, -0.5f*height_, 0.0f);
		for (int k = 0; k < N+1; ++k) {
			glVertex3f(radius_*cos(k*step), -0.5f*height_, radius_*sin(k*step));
		}
		glEnd();
	}

	if (top_) {
		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.5f*height_, 0.0f);
		for (int k = N; k > -1; --k) {
			glVertex3f(radius_*cos(k*step), 0.5f*height_, radius_*sin(k*step));
		}
		glEnd();
	}
}

void X3GroupingNode::Print(std::ostream& ost, int offset) const {
  ost << std::string(offset, ' ') << "{" << std::endl;
  for(int i = 0; i < (int)children_.size(); ++i)
    children_[i]->Print(ost, offset+2);
  ost << std::string(offset, ' ') << "}" << std::endl;
}

void X3GroupingNode::Render() const {
  for(int i = 0; i < (int)children_.size(); ++i)
    children_[i]->Render();
}

void X3GroupingNode::SetupLights(int* light_count) const {
  for(int i = 0; i < (int)children_.size(); ++i)
    children_[i]->SetupLights(light_count);
}

X3IndexedFaceSet::X3IndexedFaceSet(const char** atts) 
  : coordinate_(NULL)
{
  if(atts==0)
    return;
  for(const char** ref = atts; *ref!=NULL; ++ref) {
    const char* name = *ref;
    ++ref;
    if(*ref==NULL) {
      break;
    }
    string ref_copy(*ref);
    erase_char(ref_copy, ',');
    if(strcmp(name, "coordIndex")==0) {
      coord_index_.clear();
      istringstream ist(ref_copy);
      while(ist.good()) {
        int index;
        ist >> index;
        if(!ist.fail()) {
          coord_index_.push_back(index);
        }
      }
    }
  }
  // Now pre-process the list into triangles and quads, ignore everything else.
  int istart = 0;
  for(int i = 0; i <= (int)coord_index_.size(); ++i) {
    if(i==(int)coord_index_.size() || coord_index_[i]==-1) {
      switch(i-istart) {
        case 3:
          triangles_.push_back(XVec3i(coord_index_[istart], 
                                      coord_index_[istart+1], 
                                      coord_index_[istart+2]));
          break;
        case 4:
          quads_.push_back(XVec4i(coord_index_[istart], 
                                  coord_index_[istart+1], 
                                  coord_index_[istart+2], 
                                  coord_index_[istart+3]));
          break;
      }
      istart = i+1;
    }
  }
}

XVec3f polyNormal(const XVec3f & p1, const XVec3f & p2, const XVec3f & p3)
{
	XVec3f normal = (p2 - p1).cross(p3 - p2);
	normal.normalize();
	return normal;
}
	
	
void X3IndexedFaceSet::Render() const {
  // YOUR CODE HERE: Modify this function to render sets of triangles 
  // and sets of quads.
  if(!coordinate_)
    return;

  glBegin(GL_TRIANGLES);
  for (std::vector<XVec3i>::const_iterator it = triangles_.begin();
			it != triangles_.end(); it++) {
	  for (int i = 0; i < NUM_TRI_SIDES; i++) {
		  glNormal3fv(normals_[(*it)(i)]);
		  glVertex3fv(coordinate_->point((*it)(i)));
	  }
  }
  glEnd();
  glBegin(GL_QUADS);
  for (std::vector<XVec4i>::const_iterator it = quads_.begin();
		  it != quads_.end(); it++) {
	  for (int i = 0; i < NUM_QUAD_SIDES; i++) {
		  glNormal3fv(normals_[(*it)(i)]);
		  glVertex3fv(coordinate_->point((*it)(i)));
	  }
  }
  glEnd();
	

}

void X3IndexedFaceSet::Add(X3NodeType type, X3Node* node) {
  if(type==X3NODE_COORDINATE) {
    coordinate_ = reinterpret_cast<X3Coordinate*>(node);
    normals_.clear();
    normals_.resize(coordinate_->size(), XVec3f(0.0f, 0.0f, 0.0f));

    // YOUR CODE HERE: Modify the code below to accumulate normals for every vertex
    // from every triangle and quad it is a part of. Go through every triangle and every quad 
    // accumulating normals for their vertices, and then normalize the normal vectors.

	//cout << "Number of coordinates: " << coordinate_->size() << endl;
	//cout << "Number of coord indices: " << coord_index_.size() << endl;
	XVec3f normal;

	// Calculate the triangles' normal
	for (std::vector<XVec3i>::iterator it = triangles_.begin();
			it != triangles_.end(); it++) {
		normal = polyNormal(coordinate_->point((*it)(0)),
								   coordinate_->point((*it)(1)),
								   coordinate_->point((*it)(2)));
		for (int i = 0; i < NUM_TRI_SIDES; i++)
			normals_[(*it)(i)] += normal;
	}

	// Calculate the quad normal
	for (std::vector<XVec4i>::iterator it = quads_.begin();
			it != quads_.end(); it++) {
		normal = polyNormal(coordinate_->point((*it)(0)),
								   coordinate_->point((*it)(1)),
								   coordinate_->point((*it)(2)));
		for (int i = 0; i < NUM_QUAD_SIDES; i++)
			normals_[(*it)(i)] += normal;
	}

	// Normalize each normal
	for (std::vector<XVec3f>::iterator it = normals_.begin();
			it != normals_.end(); it++) {
		it->normalize();
	}

  } else {
    X3Node::Add(type, node);
  }
}

X3LightNode::X3LightNode(const char** atts) 
  : ambient_intensity_(0.0f),
    color_(1.0f, 1.0f, 1.0f, 1.0f),
    intensity_(1.0f)
{
  if(atts==0)
    return;
  for(const char** ref = atts; *ref!=NULL; ++ref) {
    const char* name = *ref;
    ++ref;
    if(*ref==NULL) {
      break;
    }
    if(strcmp(name, "color")==0) {
      sscanf(*ref, "%f %f %f", &color_(0), &color_(1), &color_(2));
    } else if(strcmp(name, "ambientIntensity")==0) {
      sscanf(*ref, "%f", &ambient_intensity_);
    } else if(strcmp(name, "intensity")==0) {
      sscanf(*ref, "%f", &intensity_);
    }
  }
}

void X3LightNode::SetupLights(int* light_count) const {
  glEnable(GL_LIGHT0 + *light_count);
  ++(*light_count);
}


X3Material::X3Material(const char** atts) 
  : ambient_intensity_(0.2f),
    diffuse_color_(0.8f, 0.8f, 0.8f, 1.0f),
    emissive_color_(0.0f, 0.0f, 0.0f, 1.0f),
    shininess_(0.2f),
    specular_color_(0.0f, 0.0f, 0.0f, 1.0f)
{
  if(atts==0)
    return;
  for(const char** ref = atts; *ref!=NULL; ++ref) {
    const char* name = *ref;
    ++ref;
    if(*ref==NULL) {
      break;
    }
    if(strcmp(name, "diffuseColor")==0) {
      sscanf(*ref, "%f %f %f", &diffuse_color_(0), &diffuse_color_(1), &diffuse_color_(2));
    } else if(strcmp(name, "specularColor")==0) {
      sscanf(*ref, "%f %f %f", &specular_color_(0), &specular_color_(1), &specular_color_(2));
    } else if(strcmp(name, "emissiveColor")==0) {
      sscanf(*ref, "%f %f %f", &emissive_color_(0), &emissive_color_(1), &emissive_color_(2));
    } else if(strcmp(name, "ambientIntensity")==0) {
      sscanf(*ref, "%f", &ambient_intensity_);
    } else if(strcmp(name, "shininess")==0) {
      sscanf(*ref, "%f", &shininess_);
    }
  }
}

void X3Material::Print(std::ostream& ost, int offset) const {
  ost << std::string(offset, ' ') << Name();
  ost << ": diffuse=( " << diffuse_color_ << ")" 
    << ", specular=( " << specular_color_ << ")"
    << ", emissive=( " << emissive_color_ << ")"
    << ", shininess= " << shininess_ << ""
    << ", ambient= " << ambient_intensity_ << "."
    << std::endl;
}

void X3Material::Render() const {
  // YOUR CODE HERE: Modify this function to properly 
  // setup OpenGL material properties.
	glMaterialf(GL_FRONT, GL_AMBIENT, ambient_intensity_);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_color_);
	glMaterialfv(GL_FRONT, GL_EMISSION, emissive_color_);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess_ * 128);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_color_);
}

X3PointLight::X3PointLight(const char** atts) 
  : attenuation_(1.0f, 0.0f, 0.0f),
    location_(0.0f, 0.0f, 0.0f),
    X3LightNode(atts)
{
  if(atts==0)
    return;
  for(const char** ref = atts; *ref!=NULL; ++ref) {
    const char* name = *ref;
    ++ref;
    if(*ref==NULL) {
      break;
    }
    if(strcmp(name, "attenuation")==0) {
      sscanf(*ref, "%f %f %f", &attenuation_(0), &attenuation_(1), &attenuation_(2));
    } else if(strcmp(name, "location")==0) {
      sscanf(*ref, "%f %f %f", &location_(0), &location_(1), &location_(2));
    }
  }
}

void X3PointLight::SetupLights(int* light_count) const {
  // YOUR CODE HERE: Modify this function to properly 
  // setup point light properties as described in section 17.2 of X3D specs.
  // You need to be setting up light with id GL_LIGHT0 + *light_count
	GLenum light = GL_LIGHT0 + *light_count;
	glLightf(light, GL_AMBIENT, X3LightNode::ambient_intensity());
	glLightf(light, GL_DIFFUSE, X3LightNode::intensity());
	glLightfv(light, GL_POSITION, location_);


  // Keep this call at the end to do proper light counting.
  X3LightNode::SetupLights(light_count);
}

void X3Scene::Add(X3NodeType type, X3Node* node) {
  if(type==X3NODE_VIEWPOINT) {
    if(viewpoint_)
      delete viewpoint_;
    viewpoint_ = reinterpret_cast<X3Viewpoint*>(node);
  } else {
    X3GroupingNode::Add(type, node);
  }
}

// Scene rendering: first setup viewing transform then render the scene contents.
void X3Scene::Render() const {
  if(viewpoint_)
    viewpoint_->Render();
  X3GroupingNode::Render();
}

// Scene lighting setup: first setup viewing transform then traverse the scene contents
// looking for lights.
void X3Scene::SetupLights(int* light_count) const {
  if(viewpoint_)
    viewpoint_->SetupLights(light_count);
  X3GroupingNode::SetupLights(light_count);
}

void X3Scene::Print(std::ostream& ost, int offset) const {
  ost << std::string(offset, ' ') << Name() << std::endl;
  if(viewpoint_)
    viewpoint_->Print(ost, offset+1);
  X3GroupingNode::Print(ost, offset);
}

void X3Shape::Print(std::ostream& ost, int offset) const {
  ost << std::string(offset, ' ') << Name() << std::endl;
  if(appearance_)
    appearance_->Print(ost, offset+1);
  if(geometry_)
    geometry_->Print(ost, offset+1);
}

// If you are adding a new type of geometry node it needs to be accounted for
// here so that the shape class knows about it.
void X3Shape::Add(X3NodeType type, X3Node* node) {
  if(type==X3NODE_BOX || type==X3NODE_CYLINDER 
    || type==X3NODE_CONE || type==X3NODE_INDEXEDFACESET) {
    geometry_ = reinterpret_cast<X3GeometryNode*>(node);
  } else if(type==X3NODE_APPEARANCE) {
    appearance_ = reinterpret_cast<X3Appearance*>(node);
  } else {
    X3Node::Add(type, node);
  }
}

// Shape rendering: setup the material in the appearance then proceed to render
// the geometry. 
// For the second assignment: if there is no appearance for the node, then its material
// properties are undefined. You can assume that each shape will have appearance set up
// explicitly.
void X3Shape::Render() const {
  if(appearance_)
    appearance_->Render();
  if(geometry_)
    geometry_->Render();
}

// Transform grouping node.
X3Transform::X3Transform(const char** atts) 
  : translation_(0.0f, 0.0f, 0.0f),
    rotation_(1.0f, 0.0f, 0.0f, 0.0f),
    scale_(1.0f, 1.0f, 1.0f),
    center_(0.0f, 0.0f, 0.0f)
{
  if(atts==0)
    return;
  for(const char** ref = atts; *ref!=NULL; ++ref) {
    const char* name = *ref;
    ++ref;
    if(*ref==NULL) {
      break;
    }
    if(strcmp(name, "translation")==0) {
      sscanf(*ref, "%f %f %f", &translation_(0), &translation_(1), &translation_(2));
    } else if(strcmp(name, "rotation")==0) {
      sscanf(*ref, "%f %f %f %f", &rotation_.axis(0), &rotation_.axis(1), 
        &rotation_.axis(2), &rotation_.angle_rad);
    } else if(strcmp(name, "scale")==0) {
      sscanf(*ref, "%f %f %f", &scale_(0), &scale_(1), &scale_(2));
    } else if(strcmp(name, "center")==0) {
      sscanf(*ref, "%f %f %f", &center_(0), &center_(1), &center_(2));
    }
  }
}

void X3Transform::Render() const {
  // YOUR CODE HERE: Modify this function to properly push and pop the state
  // and to setup the transforms for rendering its children.
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(translation_(0), translation_(1), translation_(2));
	glTranslatef(center_(0), center_(1), center_(2));
	glRotatef(rotation_.angle_rad, rotation_.axis(0), rotation_.axis(1), rotation_.axis(2));
	glScalef(scale_(0), scale_(1), scale_(2));
	glTranslatef(-center_(0), -center_(1), -center_(2));
	X3GroupingNode::Render();
	glPopMatrix();
}

void X3Transform::SetupLights(int* light_count) const {
  // YOUR CODE HERE: Modify this function to properly setup
  // lights at their transformed locations.
  // This function should be very similar to the Render function.
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(translation_(0), translation_(1), translation_(2));
	glTranslatef(center_(0), center_(1), center_(2));
	glRotatef(rotation_.angle_rad, rotation_.axis(0), rotation_.axis(1), rotation_.axis(2));
	glScalef(scale_(0), scale_(1), scale_(2));
	glTranslatef(-center_(0), -center_(1), -center_(2));
	X3GroupingNode::SetupLights(light_count);
	glPopMatrix();
}

X3Viewpoint::X3Viewpoint(const char** atts) 
  : position_(0.0f, 0.0f, 10.0f),
    phi_(0.0f), psi_(0.0f)
{
  if(atts==0)
    return;
  for(const char** ref = atts; *ref!=NULL; ++ref) {
    const char* name = *ref;
    ++ref;
    if(*ref==NULL) {
      break;
    }
    if(strcmp(name, "position")==0) {
      sscanf(*ref, "%f %f %f", &position_(0), &position_(1), &position_(2));
    }
  }
}

void X3Viewpoint::Print(std::ostream& ost, int offset) const {
  ost << std::string(offset, ' ') << Name();
  ost << ": pos=( " << position_ << ")" 
    << std::endl;
}

void X3Viewpoint::Render() const {
  // YOUR CODE HERE: Modify this function to setup viewing transformation.

  gluLookAt(position_(0), position_(1), position_(2), 0.0f, 0.0f, 0.0f, 
    0.0f, 1.0f, 0.0f);
  glRotatef(phi_, 0.0f, 1.0f, 0.0f);
  glRotatef(psi_, 1.0f, 0.0f, 0.0f);
}
