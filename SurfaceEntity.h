//=================================
// include guard
#ifndef __SURFACEENTITY_H_INCLUDED__
#define __SURFACEENTITY_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "CartesianFrame.h"
#include "RefractiveIndex.h"
#include "ReflectionProperties.h"
#include "ColourProperties.h"

#include "SurfaceProperties.h"
#include "VolumeProperties.h"
//=================================
class SurfaceEntity :public CartesianFrame {

	const VolumeProperties* outer_medium = nullptr;
	const VolumeProperties* inner_medium = nullptr;

	const SurfaceProperties *outer_surface = nullptr;
	const SurfaceProperties *inner_surface = nullptr;

	const CartesianFrame* allowed_frame_to_propagate_to = nullptr;
public:

	void set_outer_surface(const SurfaceProperties *outer_surface);
	void set_inner_surface(const SurfaceProperties *inner_surface);
	bool has_inner_surface()const;
	bool has_outer_surface()const;
	const SurfaceProperties* get_outer_surface()const;
	const SurfaceProperties* get_inner_surface()const;

	void set_outer_medium(const VolumeProperties* outer_medium);
	void set_inner_medium(const VolumeProperties* inner_medium);
	bool has_inner_medium()const;
	bool has_outer_medium()const;
	const VolumeProperties* get_outer_medium()const;
	const VolumeProperties* get_inner_medium()const;	

	bool has_restrictions_on_frames_to_propagate_to()const;
	const CartesianFrame* get_allowed_frame_to_propagate_to()const;
	void set_allowed_frames_to_propagate_to(const CartesianFrame* frame);
	std::string get_print()const;
protected:

	std::string get_surface_print()const;
};
#endif // __SURFACEENTITY_H_INCLUDED__ 
