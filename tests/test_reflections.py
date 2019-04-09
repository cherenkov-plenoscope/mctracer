# coding: utf-8
import merlict as ml
import pytest

def test_PhotonTest_Reflections_merlict():
    '''
    python version of this test case:
    https://github.com/cherenkov-plenoscope/merlict_development_kit/blob/770cd2769c5a36585a498ec2ee1ae5eb2dd17cb9/merlict/tests/PhotonTest.cpp#L84
    '''

    optical_table = ml.Frame()
    setup = ml.PropagationConfig()
    scenery = ml.Scenery()

    def add_plane_to_scenery(scenery, *args):
        '''
        args could be:
        "optical_table", ml.VEC3_ORIGIN, ml.ROT3_UNITY
        '''
        p = ml.Plane()
        p.set_name_pos_rot(*args)
        r = scenery.root
        r.children.append(p.this)
        p.mother = r.this

    add_plane_to_scenery(
        scenery,
        "optical_table",
        ml.VEC3_ORIGIN, ml.ROT3_UNITY
    )

    reflection_coefficient = 0.42

    function_paramters = ml.VectorOfVectorOfDoubles([
        [200e-9, reflection_coefficient],
        [1200e-9, reflection_coefficient],
    ])

    scenery.functions.add(
        "mirror_reflection",
        ml.Func1(function_paramters)
    )
    scenery.colors.add("mirror_color", ml.Color(200, 64, 64))

    mirror = ml.Plane()
    mirror.outer_color = scenery.colors.get("mirror_color")
    mirror.inner_color = scenery.colors.get("mirror_color")
    mirror.outer_reflection = scenery.functions.get("mirror_reflection")
    mirror.inner_reflection = scenery.functions.get("mirror_reflection")
    mirror.set_x_y_width(1.0, 1.0)
    mirror.set_name_pos_rot(
        "mirror",
        ml.VEC3_ORIGIN, ml.Rot3(0, ml.deg2rad(90), ml.deg2rad(45))
    )

    optical_table.root().children.append(mirror.this)
    mirror.mother = optical_table.root().this

    scenery.colors.add("absorber_color", ml.Color(50, 50, 50))
    absorber = ml.Plane()
    absorber.outer_color = scenery.colors.get("absorber_color")
    absorber.inner_color = scenery.colors.get("absorber_color")
    absorber.set_x_y_width(1.0, 1.0)
    absorber.set_name_pos_rot(
        "absorber",
        ml.Vec3(0, 2, 0),
        ml.Rot3(ml.deg2rad(90), 0, 0)
    )

    optical_table.root().children.append(absorber.this)
    absorber.mother = optical_table.root().this

    absorber_sensor = ml.Sensor(0, absorber)
    sensors_vector = ml.VectorOfSensors((absorber_sensor,))
    sensors = ml.Sensors(sensors_vector)

    scenery.root.init_tree_based_on_mother_child_relations()

    support = ml.Vec3(-2.0, 0.0, 0.0)
    direction = ml.VEC3_UNIT_X
    prng = ml.Mt19937()
    photons = ml.VectorOfPhotons()
    num_phot = 1e4
    for i in range(int(num_phot)):
        wavelength = i / num_phot * 100e-9 + 250e-9
        P = ml.Photon(support, direction, wavelength)
        # P.simulation_truth_id = i  # raises TypeError
        photons.push_back(P)

    ml.propagate_photons_in_frame_with_config(
        photons, scenery.root, setup, prng
    )

    sensors.assign_photons(photons)

    ph_reached_sensor = absorber_sensor.photon_arrival_history.size()
    ph_emitted = photons.size()

    print(
        'ph_reached_sensor = absorber_sensor.photon_arrival_history.size():\n',
        ph_reached_sensor
    )

    print(
        'ph_emitted = photons.size():\n',
        ph_emitted
    )

    assert reflection_coefficient == pytest.approx(
        ph_reached_sensor / ph_emitted,
        rel=2e-3
    )
