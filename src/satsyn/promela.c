#include "promela.h"

  void
oput_promela_state_XnSys(FildeshO* out, const XnSys* sys, XnSz sidx)
{
  for (unsigned i = 0; i < sys->vbls.sz; ++i) {
    XnEVbl x;
    x.vbl = &sys->vbls.s[i];
    x.val = sidx / x.vbl->stepsz;
    sidx = sidx % x.vbl->stepsz;
    if (i > 0)  puts_FildeshO(out, " && ");
    oput_XnEVbl(out, &x, "==");
  }
}

  void
oput_promela_XnRule(FildeshO* out, const XnRule* g, const XnSys* sys)
{
  bool had;
  XnPc* pc = &sys->pcs.s[g->pc];
  TableT(uint) t;
  puts_FildeshO(out, "/*P");
  print_int_FildeshO(out, (int)g->pc);
  puts_FildeshO(out, "*/ ");

  t = rvbls_XnPc (pc);
  had = false;

#if 1
  for (unsigned j = 0; j < t.sz; ++j) {
    XnEVbl x;
    x.vbl = &sys->vbls.s[pc->vbls.s[j]];
    x.val = g->p.s[j];
    if (had)  puts_FildeshO(out, " && ");
    had = true;
    oput_XnEVbl(out, &x, "==");
  }

  puts_FildeshO(out, " ->");

  t = wvbls_XnPc (pc);
  for (unsigned j = 0; j < t.sz; ++j) {
    XnEVbl x;
    putc_FildeshO(out, ' ');
    x.vbl = &sys->vbls.s[pc->vbls.s[j]];
    x.val = g->q.s[j];
    oput_XnEVbl(out, &x, "=");
    putc_FildeshO(out, ';');
  }
#else
  for (unsigned i = 0; i < sys->vbls.sz; ++i) {
    for (unsigned j = 0; j < t.sz; ++j) {
      if (t.s[j] == i) {
        XnEVbl x;
        x.vbl = &sys->vbls.s[i];
        x.val = g->p.s[j];
        if (had)  puts_FildeshO(out, " && ");
        had = true;
        oput_XnEVbl(out, &x, "==");
      }
    }
  }

  puts_FildeshO(out, " ->");

  t = wvbls_XnPc (pc);
  for (unsigned i = 0; i < sys->vbls.sz; ++i) {
    for (unsigned j = 0; j < t.sz; ++j) {
      if (t.s[j] == i) {
        XnEVbl x;
        putc_FildeshO(out, ' ');
        x.vbl = &sys->vbls.s[i];
        x.val = g->q.s[j];
        oput_XnEVbl(out, &x, "=");
        putc_FildeshO(out, ';');
      }
    }
  }
#endif
}
  void
oput_promela_select(FildeshO* out, const XnVbl* vbl)
{
  XnEVbl x;
  x.vbl = vbl;
  puts_FildeshO(out, "if\n");
  for (unsigned i = 0; i < vbl->domsz; ++i) {
    x.val = i;
    puts_FildeshO(out, ":: true -> ");
    oput_XnEVbl(out, &x, "=");
    puts_FildeshO(out, ";\n");
  }

  puts_FildeshO(out, "fi;\n");
}

  void
oput_promela_pc(FildeshO* out, const XnPc* pc, const XnSys* sys,
                const TableT(XnRule) rules)
{
  const unsigned pcidx = IdxEltTable (sys->pcs, pc);
  puts_FildeshO(out, "proctype P");
  print_int_FildeshO(out, (int)pcidx);
  puts_FildeshO(out, " ()\n{\n");

  {
    bool found = false;
    for (XnSz i = 0; i < rules.sz; ++i)
      if (rules.s[i].pc == pcidx)
        found = true;
    if (!found) {
      puts_FildeshO(out, "skip;\n}\n\n");
      return;
    }
  }

  puts_FildeshO(out, "end_");
  print_int_FildeshO(out, (int)pcidx);
  puts_FildeshO(out, ":\n");
  puts_FildeshO(out, "do\n");
  for (unsigned i = 0; i < rules.sz; ++i) {
    const XnRule* g = &rules.s[i];
    if (g->pc == pcidx) {
      puts_FildeshO(out, ":: atomic {");
      oput_promela_XnRule(out, g, sys);
      puts_FildeshO(out, "};\n");
    }
  }
  puts_FildeshO(out, "od;\n");
  puts_FildeshO(out, "}\n\n");
}

  void
oput_promela(FildeshO* out, const XnSys* sys, const TableT(XnRule) rules)
{
#define oputl(s)  puts_FildeshO(out, s); putc_FildeshO(out, '\n')
  oputl( "/*** Use acceptance cycle check with the LTL claim for a full verification!" );
  oputl( " *** Assertions, end states, and progress conditions are present to help debugging." );
  oputl( " *** A safety check and liveness check (BOTH WITH LTL CLAIM DISABLED) should be" );
  oputl( " *** equivalent to verifying the LTL claim holds via the acceptance cycle check." );
  oputl( " ***/" );
  oputl( "bool Legit = false;" );
  for (unsigned i = 0; i < sys->vbls.sz; ++i) {
    const XnVbl* x = &sys->vbls.s[i];
    if (x->domsz <= 2)
      puts_FildeshO(out, "bit");
    else
      puts_FildeshO(out, "byte");

    putc_FildeshO(out, ' ');
    puts_FildeshO(out, ccstr_of_AlphaTab(&x->name));
    puts_FildeshO(out, ";\n");
  }

  for (unsigned i = 0; i < sys->pcs.sz; ++i)
    oput_promela_pc(out, &sys->pcs.s[i], sys, rules);

  oputl( "init {" );
  for (unsigned i = 0; i < sys->vbls.sz; ++i) {
    const XnVbl* x = &sys->vbls.s[i];
    oput_promela_select(out, x);
  }

  for (unsigned i = 0; i < sys->pcs.sz; ++i) {
    puts_FildeshO(out, "run P");
    print_int_FildeshO(out, (int)i);
    puts_FildeshO(out, " ();\n");
  }

  oputl( "if" );
  for (unsigned i = 0; i < sys->legit.sz; ++i) {
    if (test_BitTable (sys->legit, i)) {
      puts_FildeshO(out, ":: ");
      oput_promela_state_XnSys(out, sys, i);
      puts_FildeshO(out, " -> skip;\n");
    }
  }
  oputl( "fi;" );

  oputl( "Legit = true;" );
  oputl( "progress: skip;" );

  oputl( "end:" );
  oputl( "if" );
  for (size_t i = 0; i < sys->legit.sz; ++i) {
    if (!test_BitTable (sys->legit, i)) {
      puts_FildeshO(out, ":: ");
      oput_promela_state_XnSys(out, sys, i);
      puts_FildeshO(out, " -> skip;\n");
    }
  }
  oputl( "fi;" );
  oputl( "Legit = false;" );
  oputl( "assert(0);" );
  oputl( "}" );

  oputl( "ltl {" );
  oputl( "<> Legit && [] (Legit -> [] Legit)" );
  oputl( "}" );
#undef oputl
}

