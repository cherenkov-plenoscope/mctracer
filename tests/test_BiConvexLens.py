import pytest
import numpy as np
import merlict as ml


def make_environment_with_lens_for_test():

    scenery = ml.Scenery()
    scenery.this.disown()

    prng = ml.Mt19937()
    prng.this.disown()

    # is being returned
    env = ml.PropagationEnvironment()
    sensor_list = ml.Sensors()

    settings = ml.PropagationConfig()
    settings.max_num_interactions_per_photon = 5
    settings.this.disown()

    test_bench = scenery.root.add_frame()
    test_bench.set_name_pos_rot(
        "BiConvexLens_test_world",
        ml.VEC3_ORIGIN,
        ml.ROT3_UNITY
    )
    test_bench.this.disown()

    cfg = ml.lens_maker_Config()
    cfg.focal_length = 1.0
    cfg.aperture_radius = 0.125
    cfg.refractive_index = 1.49
    cfg.this.disown()

    lens = test_bench.add_bi_convex_lens()
    lens.set_name_pos_rot(
        "little_lens",
        ml.VEC3_ORIGIN,
        ml.ROT3_UNITY)

    scenery.colors.add("lens_color", ml.Color(255, 128, 128))

    function_paramters = ml.VectorOfVectorOfDoubles([
        [200e-9, cfg.refractive_index],
        [1200e-9, cfg.refractive_index],
    ])

    scenery.functions.add(
        "refraction_vs_wavelength",
        ml.Func1(function_paramters)
    )

    lens.outer_color = scenery.colors.get("lens_color")
    lens.inner_color = scenery.colors.get("lens_color")
    lens.inner_refraction = scenery.functions.get("refraction_vs_wavelength")
    lens.set_curvature_radius_and_aperture_radius(
        ml.get_curvature_radius(cfg),
        cfg.aperture_radius
    )
    lens.this.disown()

    image_sensor = test_bench.add_disc()
    image_sensor.this.disown()
    image_sensor.set_name_pos_rot(
        "sensor_disc",
        ml.Vec3(0.0, 0.0, -1.0),
        ml.ROT3_UNITY)

    scenery.colors.add("sensor_color", ml.Color(128, 255, 128))
    image_sensor.outer_color = scenery.colors.get("sensor_color")
    image_sensor.inner_color = scenery.colors.get("sensor_color")
    image_sensor.set_radius(cfg.aperture_radius)

    scenery.sensors.add(0, image_sensor)
    sensor_list.init(scenery.sensors.sensors)

    scenery.root.init_tree_based_on_mother_child_relations()

    prng.set_seed(0)

    env.root_frame = scenery.root
    env.config = settings
    env.prng = prng

    return env, sensor_list


def test_send_photon_frontal_into_lens():
    total_propagations = int(1e4)
    num_photons_reaching_sensor_disc = 0

    env, _ = make_environment_with_lens_for_test()

    for _ in range(total_propagations):

        blue_photon = ml.Photon(
            ml.Vec3(0.0, 0.0, 1.0),
            ml.Vec3(0.0, 0.0, -1.0),
            433e-9)

        ml.Propagator(blue_photon, env)

        if np.isclose(blue_photon.accumulated_distance(), 2.0):
            num_photons_reaching_sensor_disc += 1

    assert (
        num_photons_reaching_sensor_disc / total_propagations ==
        pytest.approx(0.97, abs=5e-2)
    )


def test_send_photons_frontal_into_lens_with_offset():

    env, sensor_list = make_environment_with_lens_for_test()

    num_photons_emitted = int(1e4)
    prng = ml.Mt19937(0)

    photons = ml.parallel_towards_z_from_xy_disc(
        0.125,
        num_photons_emitted,
        prng
    )

    trafo = ml.HomTra3()
    trafo.set_transformation(
        ml.Rot3(0.0, -ml.deg2rad(180.0), 0.0),
        ml.Vec3(0.0, 0.0, 1.0)
    )

    for i in range(len(photons)):
        photons[i].transform(trafo)

    ml.propagate_photons_in_frame_with_config(
        photons,
        env.root_frame,
        env.config,
        prng)

    sensor_list.clear_history()
    sensor_list.assign_photons(photons)

    assert len(sensor_list.by_frame) == 1

    first_sensor = sensor_list.at(0)

    assert (
        ml.point_spread_std_dev(first_sensor.photon_arrival_history) ==
        pytest.approx(1.5e-3, abs=1e-3)
    )

    assert (
        len(first_sensor.photon_arrival_history) / num_photons_emitted ==
        pytest.approx(1, abs=1e-1)
    )
