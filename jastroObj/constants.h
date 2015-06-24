#ifndef CONSTANTS_H
#define CONSTANTS_H

#define N_ASPECTS 10
                                        /* sun, moon, mercury, venus, mars, jupiter, saturn, uranus */
#define N_PLANETS 16                    /* neptune, pluto, node, node, lilith, lilith, earth, chiron*/
#define MAX_ASPECTS 50
#define MAX_SYNASTRIE_PLANETARY_ASPECTS 80
#define MAX_SYNASTRIE_AXES_ASPECTS 20
#define MAX_SYNASTRIE_AXES_REVERSE_ASPECTS 20

namespace saturnia {

// letters under which astro symbols are accessed in astro font

const QString signLetters [] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L"};

const QString signNames [] = {"Aries", "Taurus", "Twin", "Cancer", "Leo", "Virgo", "Libra", "Scorpio",
                              "Sagittarius", "Capricorn", "Aquarius", "Pisces"};

                            //  sun  moon mercury venus mars jupiter saturn uranus neptune pluto node true node lilith true lilith  earth chiron
const QString planetLetters [] = {"Q", "R","S",     "T ", "U", "V",    "W",   "X",   "Y",    "Z",  "<", "<",      "\\",  "\\",        "]",  "["};

    //
const QString planetNames [] = {"Sun","Moon","Mercury","Venus","Mars","Jupiter","Saturn","Uranus","Neptune","Pluto",
                                 "Node","True Node","Lilith","True Lilith","Earth", "Chiron"};


enum Month {
    January = 1,
    February,
    March,
    April,
    May,
    June,
    July,
    August,
    September,
    October,
    November,
    December
};

enum AspectType {
    ASPECT_KONJUNCTION =    0,
    ASPECT_SEMISEXTILE =    1,
    ASPECT_SEXTILE =        2,
    ASPECT_SQUARE =         3,
    ASPECT_TRINE =          4,
    ASPECT_QUINCUNX =       5,
    ASPECT_OPPOSITION =     6,
    ASPECT_SEMISQUARE =     7,
    ASPECT_QUINTILE =       8,
    ASPECT_BIQUINTILE =     9
};

enum TimeZone {
    IDLW,
    HAST,
    AKST,
    PST,
    MST,
    CST,
    EST,
    AST,
    NST,
    GMT,
    MEZ,
    EET,
    MSK,
    BT,
    IRT,
    IST,
    ICT,
    CNST,
    JST,
    ACST,
    AEST,
    NFT,
    NCST,
    UTCMinus1,
    UTCMinus2,
    UTCMinus3,
    UTCMinus9,
    UTCMinus11,
    UTCPlus4,
    UTCPlus4_5,
    UTCPlus5,
    UTCPlus5_75,
    UTCPlus6,
    UTCPlus6_5,
    UTCPlus8_75,
    UTCPlus10_5,
    UTCPlus11,
    UTCPlus12_75,
    UTCPlus13,
    UTCPlus14,
    YST,
    PT,
    VST,
    UTC,
    WET,
    CET,
    IDLE,
    NZST
};


enum SubCode {

    NONE            = 0,

    /* codes for German federal lands               */
    BAY,                /*  Bayern                  */
    BBG,                /*  Brandenburg             */
    BER,                /*  Berlin                  */
    BRE,                /*  Bremen                  */
    BW,                 /*  Baden-Württemberg       */
    HES,                /*  Hessen                  */
    HH,                 /*  Hamburg                 */
    MVP,                /*  Mecklenburg-Vorpommern  */
    NDS,                /*  Niedersachsen           */
    NRW,                /*  Nordrhein-Westfalen     */
    RPF,                /*  Rheinland-Pfalz         */
    SAA,                /*  Saarland                */
    SAC,                /*  Sachsen                 */
    SAN,                /*  Sachsen-Anhalt          */
    SWH,                /*  Schleswig-Holstein      */
    THU,                /*  Thüringen               */

    /* codes for US federal states                  */
    MI                  /* Michigan                 */

};


/* The ISO 3166 Country Codes       */
enum CountryCode {
    nowhere,
    ac, /* 	Ascension Island uk 247                                 */
    ad, /*      Andorra (UN) 376                                        */
    ae, /*	United Arab Emirates (formerly Trucial Coast) (UN) 971  */
    af, /* 	Afghanistan (UN) 93                                     */
    ag, /* 	Antigua and Barbuda qe2 (UN) 1268                       */
    ai, /* 	Anguilla qe2 1264                                       */
    al, /* 	Albania (UN) 355                                        */
    am, /* 	Armenia (Hayastan) (ussr) (UN) 374                      */
    an, /* 	Netherlands Antilles (part of NL) 599                   */
    ao, /* 	Angola (UN) 244                                         */
    aq, // 	Antarctica 672
    ar, // 	Argentina (UN) 54
    as, // 	American Samoa us 684
    at, // 	Austria (UN) 43
    au, // 	Australia qe2 (UN) 61
    aw, // 	Aruba (seceded from NL) 297
    az, // 	Azerbaijan (ussr) (UN) 994
    dz, // 	Algeria (UN) 213
    ba, // 	Bosnia and Herzegovina (yu) (UN) 387
    bb, // 	Barbados qe2 (UN) 1246
    bd, // 	Bangladesh (formerly East Pakistan) uk (UN) 880
    be, // 	Belgium (UN) 32
    bf, // 	Burkina Faso (formerly Upper Volta) (UN) 226
    bg, // 	Bulgaria (UN) 359
    bh, // 	Bahrain (once Dilmun) (UN) 973
    bi, // 	Burundi (formerly part of Ruanda-Urundi, formerly German East Africa) (UN) 257
    bj, // 	Benin (UN) 229
    bm, // 	Bermuda uk 1441
    bn, // 	Brunei Darussalam (UN) 673
    bo, // 	Bolivia (UN) 591
    br, // 	Brazil (UN) 55
    bs, // 	Bahamas qe2 (UN) 1242
    bt, // 	Bhutan (UN) 975
    bv, // 	Bouvet Island no
    bw, // 	Botswana (formerly Bechuanaland) qe2 (UN) 267
    by, // 	Belarus (ussr) (once White Russia) (UN) 375
    bz, // 	Belize (formerly British Honduras) qe2 (UN) 501
    io, // 	British Indian Ocean Territory (Chagos Islands) navy 246
    vg, // 	British Virgin Islands uk 1284
    ca, // 	Canada qe2 (UN) 1
    cc, // 	Cocos Islands (Keeling) au 61-891
    cd, // 	Congo, Democratic People's Republic (formerly ZR) (formerly Belgian Congo) (UN) 243
    cf, // 	Central African Republic (formerly Ubangi-Shari) (UN) 236
    cg, // 	Congo, Republic of (once French Congo) (UN) 242
    ch, // 	Switzerland (Schweiz in German) 41
    ci, // 	Côte d'Ivoire (Ivory Coast) (UN) 225
    ck, // 	Cook Islands nz 682
    cl, // 	Chile (UN) 56
    cm, // 	Cameroon (UN) 237
    cn, // 	China (UN) 86
    co, // 	Colombia (UN) 57
    cr, // 	Costa Rica (UN) 506
    cs, // 	Serbia and Montenegro 381
    cu, // 	Cuba (UN) 53
    cv, // 	Cape Verde Islands (UN) 238
    cx, // 	Christmas Island au 61-891
    cy, // 	Cyprus (UN) 357
    cz, // 	Czech Republic (formerly part of CS) (UN) 420
    kh, // 	Cambodia (formerly Kampuchea) (UN) 855
    ky, // 	Cayman Islands uk 1345
    td, // 	Chad (Tchad) (UN) 235
    km, // 	Comoros Islands (UN) 269
    hr, // 	Croatia (Hrvatska) (yu) (UN) 385
    dd, // 	German Democratic Republic (East Germany) (now Germany) 49
    de, // 	Deutschland (Germany) (formerly East and West Germany) (UN) 49
    dj, // 	Djibouti (formerly Afars and Issas) (UN) 253
    dk, // 	Denmark (UN) 45
    dm, // 	Dominica uk (UN) 1767
    _do, // 	Dominican Republic (UN) 1809
    ec, // 	Ecuador (UN) 593
    ee, // 	Estonia (ussr) (UN) 372
    eg, // 	Egypt (UN) 20
    eh, // 	Western Sahara (disputed by MA) (formerly Spanish Sahara) 212
    er, // 	Eritrea (once part of ET) (UN) 291
    es, // 	España (Spain) (UN) 34
    et, // 	Ethiopia (ancient Abyssinia) (UN) 251
    eu, // 	European Union
    tp, // 	East Timor (disputed by ID) 670
    sv, // 	El Salvador (UN) 503
    gq, // 	Equatorial Guinea (formerly Spanish Guinea) (UN) 240
    fi, // 	Finland (UN) 358
    fj, // 	Fiji (UN) 679
    fk, // 	Falkland Islands (formerly Malvinas) 500
    fm, // 	Federated States of Micronesia (Caroline Islands) (UN) 691
    fo, // 	Faroe Islands dk 298
    fr, // 	France (UN) 33
    fx, // 	France, Metropolitan (i.e. European) 33
    gf, // 	French Guiana fr 594
    pf, // 	French Polynesia fr (formerly Oceania) 689
    tf, // 	French Southern Territories fr
    ga, // 	Gabon (UN) 241
    gb, // 	Great Britain (ISO3166 only) 44
    gd, // 	Grenada qe2 (UN) 1473
    ge, // 	Georgia (ussr) (UN) 995
    gg, // 	Guernsey (Channel Island) uk 44
    gh, // 	Ghana (UN) 233
    gi, // 	Gibraltar uk 350
    gl, // 	Greenland dk 299
    gm, // 	Gambia, The (UN) 220
    gn, // 	Guinea (formerly French Guinea) (UN) 224
    gp, // 	Guadeloupe fr 590
    gr, // 	Greece (UN) 30
    gs, // 	Georgia, South, and The South Sandwich Islands uk
    gt, // 	Guatemala (UN) 502
    gu, // 	Guam us 1671
    gw, // 	Guinea-Bissau (formerly Portuguese Guinea) (UN) 245
    gy, // 	Guyana (formerly British Guiana) (UN) 592
    hk, // 	Hong Kong (administration of CN) 852
    hm, // 	Heard and McDonald Islands au
    hn, // 	Honduras (UN) 504
    ht, // 	Haiti (UN) 509
    hu, // 	Hungary (Magyarország) (UN) 36
    va, // 	Holy See (Vatican City State) 379
    id, //      Indonesia (UN) 62
    ie, // 	Ireland (Eire) (UN) 353
    il, // 	Israel (formerly Palestine) (ancient Canaan) (UN) 972
    im, // 	Isle of Man uk 44
    in, // 	India (UN) 91
    iq, // 	Iraq (ancient Mesopotamia) (UN) 964
    ir, // 	Iran (ancient Persia) (UN) 98
    is, // 	Iceland (UN) 354
    it, // 	Italy (ancient center of Roman Empire) (UN) 39
    je, // 	Jersey (Channel Island) uk 44
    jm, // 	Jamaica qe2 (UN) 1876
    jo, // 	Jordan (formerly Transjordan) (UN) 962
    jp, // 	Japan (Nippon) (UN) 81
    ke, // 	Kenya (UN) 254
    kg, // 	Kyrgyzstan (ussr) (UN) 996
    ki, // 	Kiribati (UN) 686
    kn, // 	Saint Kitts and Nevis qe2 (UN) 1869
    kp, //	Korea, North (Democratic People's Republic of) (UN) 850
    kr, // 	Korea, South (Republic of) (UN) 82
    kw, // 	Kuwait (UN) 965
    kz, // 	Kazakhstan (ussr) (UN) 7
    la, // 	Laos (UN) 856
    lb, // 	Lebanon (UN) 961
    lc, // 	Lucia, Saint qe2 (UN) 1758
    li, // 	Liechtenstein (UN) 41
    lk, // 	Sri Lanka (formerly Ceylon) (UN) 94
    lr, // 	Liberia (UN) 231
    ls, // 	Lesotho (formerly Basutoland) (UN) 266
    lt, // 	Lithuania (ussr) (UN) 370
    lu, // 	Luxembourg (UN) 352
    lv, // 	Latvia (ussr) (UN) 371
    ly, // 	Libya (UN) 218
    ma, // 	Morocco (UN) 212
    mc, // 	Monaco (UN) 377
    md, // 	Moldova (ussr) (once part of RO) (UN) 373
    mg, // 	Madagascar (UN) 261
    mh, // 	Marshall Islands us (UN) 692
    mk, // 	Macedonia (FYROM) (yu) (UN) 389
    ml, // 	Mali (formerly French Sudan) (UN) 223
    mm, // 	Myanmar (formerly Burma) (UN) 95
    mn, // 	Mongolia (UN) 976
    mo, // 	Macau 853
    mp, // 	Mariana Islands, Northern us 1670
    mq, // 	Martinique fr 596
    mr, // 	Mauritania (UN) 222
    ms, // 	Montserrat uk 1664
    mt, // 	Malta (UN) 356
    mu, // 	Mauritius (UN) 230
    mv, // 	Maldives (UN) 960
    mw, // 	Malawi (formerly Nyasaland) (UN) 265
    mx, // 	Mexico (UN) 52
    my, // 	Malaysia (UN) 60
    mz, // 	Mozambique (UN) 258
    yt, // 	Mayotte Island fr 269
    na, // 	 Namibia (formerly South-West Africa) (UN) 264
    nc, // 	New Caledonia fr 687
    ne, // 	Niger (UN) 227
    nf, // 	Norfolk Island au 672
    ng, // 	Nigeria (UN) 234
    ni, // 	Nicaragua (UN) 505
    nl, // 	Netherlands (formerly Holland) (UN) 31
    no, // 	Norway (UN) 47
    np, // 	Nepal (UN) 977
    nr, // 	Nauru (UN) 674
    nt, // 	Neutral Zone (IQ and SA border)
    nu, // 	Niue nz 683
    nz, // 	New Zealand qe2 (UN) 64
    om, // 	Oman (formerly Muscat and Oman) (UN) 968
    pa, // 	Panama (UN) 507
    pc, // 	Pacific Islands Trust Territory (now FM, MH, GU, PW)
    pe, // 	Peru (UN) 51
    pg, // 	Papua New Guinea qe2 (UN) 675
    ph, // 	Philippines (UN) 63
    pk, // 	Pakistan (once part of India) (UN) 92
    pl, // 	Poland (UN) 48
    pm, // 	Saint Pierre and Miquelon 508
    pn, // 	Pitcairn Island uk 872
    pr, // 	Puerto Rico us 1787
    ps, // 	Palestinian Territories (West Bank, Gaza Strip) (disputed by IL) 970
    pt, // 	Portugal (UN) 351
    pw, // 	Palau (UN) 680
    py, // 	Paraguay (UN) 595
    qa, // 	Qatar (UN) 974
    re, // 	Réunion Island fr 262
    ro, // 	Romania (UN) 40
    ru, // 	Russia (ussr) (UN) 7
    rw, // 	Rwanda (formerly part of Ruanda-Urundi, German East Africa) (UN) 250
    sa, // 	Saudi Arabia (UN) 966
    sb, // 	Solomon Islands qe2 (UN) 677
    sc, // 	Seychelles (UN) 248
    sd, // 	Sudan (UN) 249
    se, // 	Sweden (UN) 46
    sg, // 	Singapore (UN) 65
    sh, // 	Saint Helena uk 290
    si, // 	Slovenia (yu) (UN) 386
    sj, // 	Svalbard and Jan Mayen Islands no 79
    sk, // 	Slovakia (formerly part of CS) (UN) 421
    sl, // 	Sierra Leone (UN) 232
    sm, // 	San Marino (UN) 378
    sn, // 	Senegal (UN) 221
    so, // 	Somalia (UN) 252
    sr, // 	Suriname (UN) 597
    st, // 	São Tomé and Príncipe (UN) 239
    su, // 	Soviet Union (USSR, now 15 separate republics) 7
    sy, // 	Syria (UN) 963
    sz, // 	Swaziland (UN) 268
    vc, // 	Saint Vincent and The Grenadines qe2 (UN) 1784
    ws, // 	Samoa (formerly Western Samoa) (UN) 685
    za, // 	South Africa (Zuid Africa) (UN) 27
    tc, // 	Turks and Caicos Islands uk 1649
    tg, // 	Togo (formerly French Togoland) (UN) 228
    th, // 	Thailand (ancient Siam) (UN) 66
    tj, // 	Tajikistan (ussr) (UN) 992
    tk, // 	Tokelau nz 690
    tl, // 	Timor-Leste (East Timor) 670
    tm, // 	Turkmenistan (ussr) (UN) 993
    tn, // 	Tunisia (UN) 216
    to, // 	Tonga (UN) 676
    tr, // 	Turkey (once center of Ottoman Empire) (ancient Anatolia) (UN) 90
    tt, // 	Trinidad and Tobago (UN) 1868
    tv, // 	Tuvalu (formerly Ellice Islands) (UN) 688
    tw, // 	Taiwan (disputed by CN) 886
    tz, // 	Tanzania (formerly Tanganyika and Zanzibar) (UN) 255,259
    ua, // 	Ukraine (ussr) (UN) 380
    ug, // 	Uganda (UN) 256
    uk, // 	United Kingdom (Great Britain and Northern Ireland) (UN) 44
    um, // 	US Minor Outlying Islands (Midway, Wake, etc.) 808
    us, // 	United States (UN) 1
    uy, // 	Uruguay (UN) 598
    uz, // 	Uzbekistan (ussr) (UN) 998
    vi, // 	US Virgin Islands us 1340
    ve, // 	Venezuela (UN) 58
    vn, // 	Viet Nam (formerly North and South Vietnam) (UN) 84
    vu, // 	Vanuatu (formerly New Hebrides) (UN) 678
    wf, // 	Wallis and Futuna Islands fr 681
    ye, // 	Yemen (UN) 967
    yu, // 	Yugoslavia ((Serbia and Montenegro)) (formerly included BA, HR, MK, SI) (UN) 381
    zm, // 	Zambia (formerly Northern Rhodesia) (UN) 260
    zr, // 	Zaire (now CD) 243
    zw // 	Zimbabwe (formerly Southern Rhodesia) (UN) 263

};


}

#endif // CONSTANTS_H
