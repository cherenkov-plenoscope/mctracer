from __future__ import print_function
import matplotlib.pyplot as plt
import numpy as np

lightfieldcalibration_satistics = np.genfromtxt('/home/sebastian/Desktop/TheBigLebowSkyObservatory/160123/sub_pixel_statistics.txt')
event_photon_sums = np.genfromtxt('/home/sebastian/Desktop/TheBigLebowSkyObservatory/160123/paul.txt')

# pointing direction stats
cxs_all_sub_pixel = lightfieldcalibration_satistics[:,1]
cys_all_sub_pixel = lightfieldcalibration_satistics[:,2]

number_of_subpixels_per_pixel = 37
number_of_pixels = event_photon_sums.shape[1]/number_of_subpixels_per_pixel

cx = np.zeros(number_of_pixels)
cy = np.zeros(number_of_pixels)
for pix in range(number_of_pixels):
	pix_off = number_of_subpixels_per_pixel*pix
	cx[pix] = np.nanmean(cxs_all_sub_pixel[0+pix_off:number_of_subpixels_per_pixel+pix_off])
	cy[pix] = np.nanmean(cys_all_sub_pixel[0+pix_off:number_of_subpixels_per_pixel+pix_off])

sx_all_sub_pixel = lightfieldcalibration_satistics[:,3]
sy_all_sub_pixel = lightfieldcalibration_satistics[:,4]

sx = np.zeros(number_of_subpixels_per_pixel)
sy = np.zeros(number_of_subpixels_per_pixel)
for sub_pix in range(number_of_subpixels_per_pixel):
	sx[sub_pix] = np.nanmean(sx_all_sub_pixel[sub_pix::number_of_subpixels_per_pixel])
	sy[sub_pix] = np.nanmean(sy_all_sub_pixel[sub_pix::number_of_subpixels_per_pixel])

pix_radius = 0.0333*(2*np.pi/360.0)
aperture_pix_radius = 0.5*50.0/7.0
focal_length = 75.0

print("pixels ", number_of_pixels)
print("sub pixels per pixel", number_of_subpixels_per_pixel)

def get_sub_aperture(evt, i):
	return evt[i::number_of_subpixels_per_pixel]

def get_aperture(evt, i):
	return evt[i*number_of_subpixels_per_pixel:(i+1)*number_of_subpixels_per_pixel]

def get_intensity_weights_for_direction(cos_x, cos_y):
	weights = np.zeros(number_of_pixels)

	dist_x = (cos_x - cx) 
	dist_y = (cos_y - cy)
	dists = np.hypot(dist_x, dist_y)
	weights = 1.0/dists**2.0

	return weights/np.sum(weights)

def plot_aperture(ap_imae, save=False):

	mag = ap_imae.max()
	fig = plt.gcf()
	plt.axis([-28,28,-28,28])
	for p in range(number_of_subpixels_per_pixel):
		pix_value = ap_imae[p]/mag

		circle1=plt.Circle((sx[p], sy[p]), aperture_pix_radius, color=str(pix_value))
		fig.gca().add_artist(circle1)

	plt.axes().set_aspect('equal')

	if save != False:
		plt.savefig(save)
	else:
		plt.show()

def plot_lebowsky(sub_image, save=False):

	mag = sub_image.max()
	fig = plt.gcf()
	plt.axis([-3.25*(2*np.pi/360.0),3.25*(2*np.pi/360.0),-3.25*(2*np.pi/360.0),3.25*(2*np.pi/360.0)])
	for p in range(number_of_pixels):
		pix_value = sub_image[p]/mag

		circle1=plt.Circle((cx[p], cy[p]), pix_radius, color=str(pix_value))
		fig.gca().add_artist(circle1)

	plt.axes().set_aspect('equal')

	if save != False:
		plt.savefig(save)
	else:
		plt.show()

def show(evt, sub=-1, save=False):

	event = event_photon_sums[evt,:]
	max_count = event.max()
	print("Event ", evt, " total photons ", event.sum(), " max in sub_pix ", max_count)

	if sub > -1:
		sub_image = get_sub_aperture(event,sub)
		print("total photons in sub pixel", sub_image.sum())

		plot_lebowsky(sub_image, save)
	else:
		sum_image = np.zeros(number_of_pixels)
		for pix in range(number_of_pixels):
			pix_off = number_of_subpixels_per_pixel*pix
			sum_image[pix] = np.sum(event[0+pix_off:number_of_subpixels_per_pixel+pix_off])

		plot_lebowsky(sum_image, save)


# digital refocus
def refocus(img, alpha):
	#img = event_photon_sums[0]
	#alpha = 1.03
	dr_image = np.zeros(number_of_pixels)

	count = 0
	for st in range(number_of_pixels):

		s = np.tan(cx[st])*focal_length
		t = np.tan(cy[st])*focal_length

		E_st = 0.0
		for uv in range(number_of_subpixels_per_pixel):

			u = sx[uv]
			v = sy[uv]

			s_tick = u + (s-u)/alpha
			t_tick = v + (t-v)/alpha

			t_des = np.arctan(t_tick/focal_length)
			s_des = np.arctan(s_tick/focal_length)

			sub_img = get_sub_aperture(img, uv)

			weights = get_intensity_weights_for_direction(s_des, t_des)
			E_st += np.dot(weights, sub_img)

			#print(E_st)

		dr_image[st] = E_st
		count += 1
		print('refocus '+str(count)+' of '+str(number_of_pixels))
		#print('alpha '+str(alpha)+' , s '+str(s)+'m, t '+str(t)+'m, s. '+str(s_tick)+'m, t. '+str(t_tick)+'m\n') 

	return dr_image

event = event_photon_sums[0]