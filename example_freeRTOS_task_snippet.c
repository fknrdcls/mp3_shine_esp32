void shine_task(void *pvParameter) {

    shine_config_t  config;  // Pointer to shine_global_config
    shine_t         s;
    shine_global_config *sc;
    mp3buf_t        databufs[STREAM_BUFFERS];
    mp3buf_t        *databuf;
    int             written, samples_per_pass, err=0, next_buf = 0;
    
    char            *lbuf;
    BaseType_t      recvd;
    int             *_buffer, *p, i, buf_pos, source_pos, source_max_samples;
    unsigned char   *data;
    int16_t         *buffer; //Stereo max samples per pass
  
    

    shine_set_config_mpeg_defaults(&config.mpeg);

    config.mpeg.mode = JOINT_STEREO;
    config.mpeg.bitr = 256;
    config.wave.samplerate = 48000;  // 48kHz @39%-45% Single core usage :)
    config.wave.channels = 2;
    source_max_samples = 512;
    ESP_LOGI("SHINE: ", "Size shine_global_config: %d", sizeof(shine_global_config));

    if(!(s = shine_initialise(&config))) {
        ESP_LOGI("SHINE: ", "Falied to allocate shine_global_config!");
        ESP_LOGI("SHINE: ", "Size shine_global_config: %d", sizeof(shine_global_config));
        vTaskDelete(NULL);
    }
    
    // Set up streaming buffers
    sc = s;
    for (i=0;i<STREAM_BUFFERS;i++) {
       databufs[i].buf  = malloc(800); 
       databufs[i].size = 0;
    }
    
    //samples_per_pass 48k mono 1152 / stereo 1152
    //samples_per_pass 24k mono 576 / stereo 576

    check_config(&config);
    samples_per_pass = sc->mpeg.granules_per_frame * 576;

    ESP_LOGI("SHINE: ", "Samples per pass %d",samples_per_pass);

    vTaskDelay(1000/portTICK_RATE_MS);
    source_pos = buf_pos = 0;
    uint32_t sample_ctr = 0;
    int lintl[3] = {0},  lintr[3] = {0};
    int si = 0;

    //Start loop to handle encoding 

    while(1) {
        //Buffers are received from the audio capture task
        xQueueReceive(processed_soundbuffer_q, &buffer, portMAX_DELAY);
        //Encode
        data = shine_encode_buffer_interleaved(s, buffer, &written);
        
        databufs[next_buf].size = written;
        memcpy(databufs[next_buf].buf, data, written);
        databuf = &databufs[next_buf];
        //Send endoded buffer to streaming server
        recvd = xQueueSend(mp3_buffer_q, &databuf, 0);
        
        next_buf++;
        if (next_buf == STREAM_BUFFERS) next_buf = 0;
        //Let FreeRTOS do things
        vTaskDelay(5/portTICK_RATE_MS);
             
    }
    vTaskDelete(NULL);
}
