#include <pybind11/pybind11.h>

#include <pangolin/gl/gldraw.h>


namespace py = pybind11;
using namespace pybind11::literals;


namespace pangolin {

void declareGLDraw(py::module & m) {

    // m.def("glDrawColouredCube", &glDrawColouredCube,
    //     "axis_min"_a = -0.5f, "axis_max"_a = 0.5f);
    m.def("glDrawAxis",
          (void (*)(float))&pangolin::glDrawAxis);

    m.def("glDrawColouredCube",
          &pangolin::glDrawColouredCube,
          py::arg("axis_min") = -0.5f,
          py::arg("axis_max") = +0.5f);

    m.def("glDraw_x0",
          &pangolin::glDraw_x0);
    m.def("glDraw_y0",
          &pangolin::glDraw_y0);
    m.def("glDraw_z0",
          &pangolin::glDraw_z0);

    m.def("glDrawFrustum", (void (*)(GLfloat, GLfloat, GLfloat, GLfloat, int, int, GLfloat)) &pangolin::glDrawFrustum);
    m.def("glDrawFrustum", (void (*)(const Eigen::Matrix3f &, int, int, GLfloat)) &pangolin::glDrawFrustum<float>);
    m.def("glDrawFrustum", (void (*)(const Eigen::Matrix3d &, int, int, GLfloat)) &pangolin::glDrawFrustum<double>);
    m.def("glDrawFrustum", (void (*)(const Eigen::Matrix3f &, int, int, const Eigen::Matrix4f &, float)) &pangolin::glDrawFrustum<float>);
    m.def("glDrawFrustum", (void (*)(const Eigen::Matrix3d &, int, int, const Eigen::Matrix4d &, double)) &pangolin::glDrawFrustum<double>);

    m.def("glDrawTexture", (void (*)(GLenum, GLint)) &pangolin::glDrawTexture);
    m.def("glDrawTextureFlipY", (void (*)(GLenum, GLint)) &pangolin::glDrawTextureFlipY);

    m.def("glDrawAxis", (void (*)(float)) &pangolin::glDrawAxis);
    m.def("glDrawAxis", (void (*)(const Eigen::Matrix4f &, float)) &pangolin::glDrawAxis<Eigen::Matrix4f, float>);
    m.def("glDrawAxis", (void (*)(const Eigen::Matrix4d &, float)) &pangolin::glDrawAxis<Eigen::Matrix4d, float>);

    m.def("glSetFrameOfReference", (void (*)(const Eigen::Matrix4f &)) &pangolin::glSetFrameOfReference);
    m.def("glSetFrameOfReference", (void (*)(const Eigen::Matrix4d &)) &pangolin::glSetFrameOfReference);

    m.def("glUnsetFrameOfReference", &pangolin::glUnsetFrameOfReference);

    m.def("glDrawVertices", &pangolin::glDrawPoints<float, 2, std::allocator<Eigen::Vector2f> >);
    m.def("glDrawVertices", &pangolin::glDrawPoints<float, 3, std::allocator<Eigen::Vector3f> >);
    m.def("glDrawVertices", &pangolin::glDrawPoints<double, 2, std::allocator<Eigen::Vector2d> >);
    m.def("glDrawVertices", &pangolin::glDrawPoints<double, 3, std::allocator<Eigen::Vector3d> >);

    m.def("glDrawPoints", &pangolin::glDrawPoints<float, 2, std::allocator<Eigen::Vector2f> >);
    m.def("glDrawPoints", &pangolin::glDrawPoints<float, 3, std::allocator<Eigen::Vector3f> >);
    m.def("glDrawPoints", &pangolin::glDrawPoints<double, 2, std::allocator<Eigen::Vector2d> >);
    m.def("glDrawPoints", &pangolin::glDrawPoints<double, 3, std::allocator<Eigen::Vector3d> >);

    m.def("glDrawLines", &pangolin::glDrawLines<float, 2, std::allocator<Eigen::Vector2f> >);
    m.def("glDrawLines", &pangolin::glDrawLines<float, 3, std::allocator<Eigen::Vector3f> >);
    m.def("glDrawLines", &pangolin::glDrawLines<double, 2, std::allocator<Eigen::Vector2d> >);
    m.def("glDrawLines", &pangolin::glDrawLines<double, 3, std::allocator<Eigen::Vector3d> >);

    m.def("glDrawLineStrip", &pangolin::glDrawLineStrip<float, 2, std::allocator<Eigen::Vector2f> >);
    m.def("glDrawLineStrip", &pangolin::glDrawLineStrip<float, 3, std::allocator<Eigen::Vector3f> >);
    m.def("glDrawLineStrip", &pangolin::glDrawLineStrip<double, 2, std::allocator<Eigen::Vector2d> >);
    m.def("glDrawLineStrip", &pangolin::glDrawLineStrip<double, 3, std::allocator<Eigen::Vector3d> >);

    m.def("glDrawLineLoop", &pangolin::glDrawLineLoop<float, 2, std::allocator<Eigen::Vector2f> >);
    m.def("glDrawLineLoop", &pangolin::glDrawLineLoop<float, 3, std::allocator<Eigen::Vector3f> >);
    m.def("glDrawLineLoop", &pangolin::glDrawLineLoop<double, 2, std::allocator<Eigen::Vector2d> >);
    m.def("glDrawLineLoop", &pangolin::glDrawLineLoop<double, 3, std::allocator<Eigen::Vector3d> >);

    m.def("glDrawAlignedBox", (void (*)(const Eigen::AlignedBox2f &, GLenum)) &pangolin::glDrawAlignedBox<float>, pybind11::arg("box"), pybind11::arg("mode") = GL_TRIANGLE_FAN);

    //py::class_<Params, std::shared_ptr<Params>> cls(m, "Params");

    //cls.def(py::init<>());

}

}  // namespace pangolin::