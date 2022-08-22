void
centeredmaster(Monitor *m)
{
	unsigned int i, n, h, mw, mx, my, oty, ety, tw;
	float mfacts = 0, lfacts = 0, rfacts = 0;
	Client *c;

	/* count number of clients in the selected monitor */
	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		if (n < m->nmaster)
			mfacts += c->cfact;
		else if ((n - m->nmaster) % 2) 
			lfacts += c->cfact;
		else
			rfacts += c->cfact;
	}
	if (n == 0)
		return;
	if(n == 1){
		c = nexttiled(m->clients);
		resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
		return;
	}

	/* initialize areas */
	mw = m->ww;
	mx = 0;
	my = 0;
	tw = mw;

	if (n > m->nmaster) {
		/* go mfact box in the center if more than nmaster clients */
		mw = m->nmaster ? m->ww * m->mfact : 0;
		tw = m->ww - mw;

		if (n - m->nmaster > 1) {
			/* only one client */
			mx = (m->ww - mw) / 2;
			tw = (m->ww - mw) / 2;
		}
	}

	oty = 0;
	ety = 0;
	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
	if (i < m->nmaster) {
		/* nmaster clients are stacked vertically, in the center
		 * of the screen */
		h = (m->wh - my) * (c->cfact / mfacts);
		resize(c, m->wx + mx, m->wy + my, mw - 2*c->bw,
		      h - 2*c->bw, 0);
		if(my + HEIGHT(c) < m->mh)
			my += HEIGHT(c);
		mfacts -= c->cfact;
	} else {
		/* stack clients are stacked vertically */
		if ((i - m->nmaster) % 2) {
			h = (m->wh - ety) * (c->cfact / lfacts);
			if(m->nmaster == 0)
				resize(c, m->wx, m->wy + ety, tw - 2*c->bw,
			       h - 2*c->bw, 0);
			else
				resize(c, m->wx, m->wy + ety, tw - 2*c->bw,
			       h - 2*c->bw, 0);
			if(ety + HEIGHT(c) < m->mh)
				ety += HEIGHT(c);
			lfacts -= c->cfact;
		} else {
			h = (m->wh - oty) * (c->cfact / rfacts);
			resize(c, m->wx + mx + mw, m->wy + oty,
			       tw - 2*c->bw, h - 2*c->bw, 0);
			if(oty + HEIGHT(c) < m->mh)
				oty += HEIGHT(c);
			rfacts -= c->cfact;
		}
	}
}
