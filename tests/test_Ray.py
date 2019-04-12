import pytest
import merlict as ml


def test_Constructor_using_vectors():
    sup = ml.Vec3(1.2, 3.4, 5.6)
    dir = ml.Vec3(7.8, 9.0, 4.2)
    ray = ml.Ray(sup, dir)
    assert ray.support() == sup
    assert ray.direction() == dir / dir.norm()
    assert 1.0 == pytest.approx(ray.direction().norm(), abs=1e-6)


def test_position_on_ray():
    sup = ml.Vec3(1.2, 3.4, 5.6)
    dir = ml.Vec3(7.0, 9.0, 4.0)
    ray = ml.Ray(sup, dir)
    alpha = 5.0
    pos_on_ray = sup + dir / dir.norm() * alpha
    assert 0.0 == pytest.approx(
        (pos_on_ray - ray.position_at(alpha)).norm(),
        abs=1e-6
    )


def test_closest_distance_to_point():
    ray = ml.Ray(ml.VEC3_ORIGIN, ml.Vec3(0.0, 0.0, 1.0))
    assert ray.closest_distance_to_point(ml.VEC3_ORIGIN) == 0.0
    assert ray.closest_distance_to_point(ml.Vec3(1.0, 0.0, 0.0)) == 1.0
    assert ray.closest_distance_to_point(ml.Vec3(0.0, 1.0, 0.0)) == 1.0
    assert ray.closest_distance_to_point(ml.Vec3(0.0, 0.0, 1.0)) == 0.0


def test_set_support_and_direction():
    ray = ml.Ray(ml.Vec3(1.2, 3.4, 5.6), ml.Vec3(0, 0, 1))
    ray.set_support_and_direction(
        ml.Vec3(1.2, 3.4, 5.6),
        ml.Vec3(1.2, 3.4, 5.6)
    )
    assert ray.direction().norm() == 1.0
