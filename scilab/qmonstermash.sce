//Plot saved data
function QMMplotCSV( path2csv )
    data = evstr( read_csv( path2csv, ',' ) );
    x = data( :, 1 );
    y = data( :, 2 );
    
    plot( x, y );
endfunction