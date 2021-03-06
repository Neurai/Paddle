#include "paddle/platform/place.h"

namespace paddle {
namespace platform {

namespace detail {

class PlacePrinter : public boost::static_visitor<> {
 public:
  PlacePrinter(std::ostream &os) : os_(os) {}
  void operator()(const CpuPlace &) { os_ << "CpuPlace"; }
  void operator()(const GpuPlace &p) { os_ << "GpuPlace(" << p.device << ")"; }

 private:
  std::ostream &os_;
};

}  // namespace detail

static Place the_default_place;

void set_place(const Place &place) { the_default_place = place; }
const Place &get_place() { return the_default_place; }

const GpuPlace default_gpu() { return GpuPlace(0); }
const CpuPlace default_cpu() { return CpuPlace(); }

bool is_gpu_place(const Place &p) {
  return boost::apply_visitor(IsGpuPlace(), p);
}
bool is_cpu_place(const Place &p) {
  return !boost::apply_visitor(IsGpuPlace(), p);
}

bool places_are_same_class(const Place &p1, const Place &p2) {
  return is_gpu_place(p1) == is_gpu_place(p2);
}

std::ostream &operator<<(std::ostream &os, const Place &p) {
  detail::PlacePrinter printer(os);
  boost::apply_visitor(printer, p);
  return os;
}

}  // namespace platform
}  // namespace paddle
