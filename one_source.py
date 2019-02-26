import numpy as np
import os
import glob

head = "// Copyright 2018 Sebastian A. Mueller\n"


def inside_namespace(txt):
	start_first_namespace = txt.find("{", txt.find('namespace relleums')) + 1
	if start_first_namespace == 0:
		return ""
	pos = start_first_namespace
	bracket_count = 1

	# print("\n")
	while bracket_count > 0:
		# print(pos, bracket_count)
		next_o = txt.find("{", pos)
		next_c = txt.find("}", pos)
		if next_o == -1:
			next_o = 9999999999
		if next_c == -1:
			next_c = 9999999999

		if next_c < next_o:
			bracket_count -= 1
			pos = next_c + 1
		elif next_o < next_c:
			bracket_count += 1
			pos = next_o + 1
		else:
			break

	end_namespace = pos

	in_first_namespace = txt[start_first_namespace:end_namespace - 1]

	rest_of_txt = txt[end_namespace:]

	if rest_of_txt.find("namespace relleums") == -1:
	 	return in_first_namespace
	else:
		return in_first_namespace + "\n" + inside_namespace(rest_of_txt)


def all_includes(txt):
	includes = []
	for line in txt.splitlines():
		if line.find("#include <") != -1:
			includes.append(line)
	return includes


def all_usings(txt):
	usings = []
	for line in txt.splitlines():
		if line.find("using ") == 0:
			usings.append(line)
	return usings


def without_include_lines(txt):
	out = []
	for line in txt.splitlines():
		if not line.find("#include") != -1:
			out.append(line)
	return "\n".join(out)


dirs = [
	"Core/function",
	"Core/random",
	"Core",
	"Core/sensor",
	"Core/scenery",
	"Core/scenery/geometry",
	"Core/scenery/primitive",
	"Core/visual",
	"Core/visual/camera_operator",
]

s = 0
pri_queu = [
	"Vec2",
	"Vec3",
	"Rot3",
	"HomTra3",
	"Core/Color",
	"Limits",
	"Func1",
	"random/Generator",
	"random/Mt19937",
	"random/random.h",
	"Core/Frame.h",
	"Core/SurfaceEntity.h",
	"Core/Frames",
	"Core/Intersection",
	"Core/Ray.h",
	"Core/RayAndFrame",
	"Core/RayForPropagation",
	"Core/Photon.h",
	"Core/PropagationConfig",
	"Core/PropagationEnvironment",
	"Core/FresnelRefractionAndReflection.h",
	"Core/Propagator.h",
	"scenery/geometry/PrismZ.h",
	"sensor/PhotonArrival.h",
	"sensor/PhotonArrivals.h",
	"sensor/Sensor.h",
	"scenery/ResourceMap.h",
	"ColorMap",
	"SensorMap",
	"FunctionMap",
	"visual/PixelCoordinate",
	"visual/Image",
	"visual/SkyDome",
	"visual/Config",
	"visual/CameraRay",
	"visual/CameraDevice",
	"visual/camera_operator/Verbosity",
]

s = 0
go_first = {}
for pri_q in pri_queu:
	go_first[pri_q] = s
	s += 1

out_header = 'lict.h'
out_source = 'lict.cpp'

headers = []
header_includes = []

for d in dirs:
	for header in glob.glob(os.path.join(d, '*.h')):
		headers.append(header)

prior = 1000
pri_heads = []
for header in headers:
	has_pri = False
	for pri in go_first:
		if pri in header:
			pri_heads.append(go_first[pri])
			has_pri = True
	if not has_pri:
		pri_heads.append(prior)
		prior +=1

pri_heads = np.array(pri_heads)
argsort = np.argsort(pri_heads)

headers = [headers[i] for i in argsort]

sources = []
source_includes = []
source_usings = []

for header in headers:
	potentiual_source_path = os.path.splitext(header)[0]+'.cpp'
	if os.path.exists(potentiual_source_path):
		sources.append(potentiual_source_path)


for header in headers:
	with open(header, 'rt') as fin:
		txt = fin.read()
	incs = all_includes(txt)
	for inc in incs:
		header_includes.append(inc)

header_includes = list(set(header_includes))

for source in sources:
	with open(source, 'rt') as fin:
		txt = fin.read()
	incs = all_includes(txt)
	for inc in incs:
		source_includes.append(inc)
	usis = all_usings(txt)
	for usi in usis:
		source_usings.append(usi)

source_includes = list(set(source_includes))
source_usings = list(set(source_usings))


with open(out_header, 'wt') as fout:
	fout.write(head)
	for header_include in header_includes:
		fout.write(header_include + '\n')
	fout.write("\n")
	fout.write("namespace relleums {\n")
	for header in headers:
		print(header)
		with open(header, 'rt') as fin:
			h_txt = fin.read()
		fout.write(inside_namespace(h_txt))
		fout.write("\n"*2)
	fout.write("}  // namespace relleums\n")


with open(out_source, 'wt') as fout:
	fout.write(head)
	fout.write('#include "' + out_header + '"\n')
	for source_include in source_includes:
		fout.write(source_include + '\n')
	fout.write("\n")
	for source_using in source_usings:
		fout.write(source_using + '\n')
	fout.write("\n")
	fout.write("namespace relleums {\n")

	for source in sources:
		print(source)
		with open(source, 'rt') as fin:
			h_txt = fin.read()
		fout.write(without_include_lines(inside_namespace(h_txt)))
		fout.write("\n"*2)
	fout.write("}  // namespace relleums\n")

# g++ docopt/docopt.cpp lict_prop.cpp -o prop -std=gnu++11