function QMMCondensatorTest( path2csv, a )
    funcprot( 0 ); //Turn off annoying messages
    
    saved_data = evstr( read_csv( path2csv, ',' ) );
    x = saved_data( :, 1 );
    y_saved = saved_data( :, 2 );
 
    y_calculated = max( y_saved ) * (1 - %e^(x * a));
    
    f1 = scf( 1 );    //Create window id 1 and set as current
    clf( f1 );        //Clear window
    plot2d( x, [y_saved, y_calculated], [2, 3] );
    legends( ['Saved';'Calculated'], [2, 3], with_box = %t, opt = 1 );
endfunction