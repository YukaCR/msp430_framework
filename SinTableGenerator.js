sin_buffer = []
freq = 8000;
Max_Peroid = 1000;
for (var i = 0; i < (40000 / 4); i++) {
    sin_buffer.push(Math.sin(i * 2 * Math.PI / 40000));
}
sin_buffer_ = sin_buffer.map(item => item * Max_Peroid)
sin_buffer_ = sin_buffer_.map(item => Math.round(item))
sin_buffer_count = []
sin_buffer_comp = []

for(var i = 0 ; i < sin_buffer_.length; i++){
    if(sin_buffer_comp.indexOf(sin_buffer_[i])==-1){
        sin_buffer_comp.push(sin_buffer_[i]);
        sin_buffer_count.push(1);
        sin_buffer_count[sin_buffer_comp.length-2]=i;
    }
}
sin_buffer_count[sin_buffer_count.length-1]=freq/4;
console.log(JSON.stringify(sin_buffer_count))
