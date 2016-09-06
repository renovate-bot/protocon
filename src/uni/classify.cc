
extern "C" {
#include "cx/syscx.h"
}

#include "uniact.hh"
#include "unifile.hh"
#include "../pfmla.hh"

#include "cx/fileb.hh"
#include <algorithm>

#include "../namespace.hh"


/** Given a list of actions and variables x[j-1] and x[j],
 * compute the tiling constraints for column j
 * in the form of a transition formula.
 **/
static
  X::Fmla
column_xfmla(const Table<UniAct>& acts, const PFmlaVbl& x_p, const PFmlaVbl& x_j)
{
  X::Fmla xn( false );
  for (uint i = 0; i < acts.sz(); ++i) {
    const UniAct& act = acts[i];
    //    x'[j-1]==a         && x[j]==b     && x'[j]==c
    xn |= x_p.img_eq(act[0]) && x_j==act[1] && x_j.img_eq(act[2]);
  }
  return xn;
}

/** Execute me now!**/
int main(int argc, char** argv) {
  int argi = init_sysCx(&argc, &argv);

  uint min_period = 1;
  uint max_period = 0;
  if (argi >= argc)
    failout_sysCx("Please provide a maximum period.");

  if (argi + 1 < argc) {
    if (!xget_uint_cstr (&min_period, argv[argi++]))
      failout_sysCx("Failed to parse min period.");
  }

  if (!xget_uint_cstr (&max_period, argv[argi++]))
    failout_sysCx("Failed to parse max period.");

  if (argi < argc)
    failout_sysCx("Too many arguments!");

  Table<UniAct> acts;
  const uint domsz =
    xget_list(stdin_XFile (), acts);

  // Initialize formula variables.
  PFmlaCtx pfmla_ctx;
  Table<PFmlaVbl> vbls(1+max_period);
  for (uint i = 0; i < 1+max_period; ++i) {
    uint vbl_id = pfmla_ctx.add_vbl((String("x") << i), domsz);
    vbls[i] = pfmla_ctx.vbl(vbl_id);
  }

  OFile ofile( stdout_OFile () );

  // Search for livelocks.
  bool livelock_found = false;
  X::Fmla xn(true);
  for (uint j = 1; j < 1+max_period; ++j) {
    xn &= column_xfmla(acts, vbls[j-1], vbls[j]);
    skip_unless (j >= min_period);
    const X::Fmla periodic_xn =
      xn & (vbls[0]==vbls[j]) & (vbls[0].img_eq_img(vbls[j]));

    if (periodic_xn.cycle_ck(0)) {
      livelock_found = true;
      ofile << "livelock"
        << "\tperiod:" << j
        << ofile.endl();
      break;
    }
  }

  if (!livelock_found) {
    // No livelocks found? Are they even a possibility?
    if (!xn.cycle_ck(0)) {
      ofile << "silent" << ofile.endl();
    }
    else {
      ofile << "unknown" << ofile.endl();
    }
  }

  lose_sysCx();
  return 0;
}

END_NAMESPACE

int main(int argc, char** argv) {
  return PROJECT_NAMESPACE::main(argc, argv);
}
