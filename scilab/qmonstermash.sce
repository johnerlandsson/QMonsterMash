function QMMCondensatorTest( path2csv, a )
    funcprot( 0 ); //Turn off annoying messages
    
    saved_data = evstr( read_csv( path2csv, ',' ) );
    x = saved_data( :, 1 );
    y_saved = saved_data( :, 2 );
    
    y_saved_min = min( y_saved );
    y_saved_max = max( y_saved );
 
    //y_calculated = min( y_saved ) + ((max( y_saved ) - min( y_saved )) * (1 - %e^(x * a)));
    y_calculated = y_saved_min + ((y_saved_max - y_saved_min) * trans());
    //y_calculated = laplace( `%e^(-3*t)`, 1, 2 );
    f1 = scf( 1 );    //Create window id 1 and set as current
    clf( f1 );        //Clear window
    plot2d( x, [y_saved, y_calculated], [2, 3] );
    legends( ['Saved';'Calculated'], [2, 3], with_box = %t, opt = 4 );
    xlabel( 'Time (s)' );
endfunction

function transtest()
    clf()
    Hlp=iir(3,'lp','ellip',[0.1 0],[.08 .03]);
    Hbp=trans(Hlp,'bp',[0.01 0.1]);
    Hsb=trans(Hlp,'sb',[0.01 0.1])
    
    clf();gainplot([Hlp;Hbp;Hsb],1d-3,0.48);
    l=legend(['original low pass';'band pass';'stop band']);
    l.legend_location="in_lower_left";
endfunction