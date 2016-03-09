
	$(function(){
	
		/**************************************************/
        
		/*-----------Variables initialization ---------------*/
		
		/*********************/
		var created_parking;
		var state; //main function clicks-following variable
		var newlamp_marker;
 		var marker;
		var polygon;
 		var circlemarker;
		var lampByID = {};
		var parkingByID = {};
		var _lamp = null;
		var _parking = null;
		var _parking_edit = null;
		var _marker = null;
		var position = null;
		var reg_lamps = null;
		var lampItems = new L.FeatureGroup();
		var parkingItems = new L.FeatureGroup();
		var drawControl;
        /**************************************************/
        
		/*-----------Map initialization---------------*/
		
		/*********************/
		
		var map = L.map('map').setView([{% if lat and lon%} {{ lat }} , {{ lon }} {% else %} 0,0 {% endif %}], {{area_type}});

		L.tileLayer('http://{s}.tile.osm.org/{z}/{x}/{y}.png', {
    		attribution: '&copy; <a href="http://osm.org/copyright">OpenStreetMap</a> contributors'
		}).addTo(map);
        /**************************************************/
        
		/*-------INFO Panel initialization--------------------*/
		
		/**********************************/
		var info = L.control();
		info.onAdd = function (map) {
			if(html_element=="zoom"){
		    	this._div_zoom = L.DomUtil.create('div', 'info'); // create a div with a class "info"
		    	this._div_zoom.innerHTML = '<h4>Select Area to zoom in</h4>' +'<select id="area_list" >'
			    {% if area_list %}
			    	{% for area_name in area_list %}
						+'<option value="{{area_name}}">{{area_name}}</option>'
					{% endfor %}
				{% endif %}
				+'</select>';
		    	return this._div_zoom;
		    }
		    else if(html_element=="info"){
		    	this._div = L.DomUtil.create('div', 'info'); // create a div with a class "info"
		   		this.update('<h4>Click parking to display occupancy and capacity</h4>');
		    	return this._div;
			}
	
		};
		info.update = function (element) {
				this._div.innerHTML = element;
		};
		html_element="info";
		info.addTo(map);
		html_element="zoom";
		info.addTo(map);
		document.getElementById("lamp_status_table").innerHTML = "";
		send_ajax_get( {% url 'parking:lamp_register_get' %}, "count", update_info_bar, []);
		
        /**************************************************/
        
		/*-----------Draw Control initialization----------------*/

		/*************************************************/
		var drawnItems = new L.FeatureGroup();
       	map.addLayer(drawnItems);
        draw_control("init");
        /**************************************************/
        
		/*------------Map events initialization---------------*/
        
        /*************************************/
        map.on('draw:created', draw_created);
        map.on('draw:edited', draw_edited);
		map.on('draw:editstart', draw_editstart);
		map.on('draw:editstop', draw_editstop);
		map.on('draw:drawstop', draw_drawstop);
		map.on('click', OnMapClick);
        /**************************************************/
        
		/*------------Initialize map setting with predefined functions---------------*/
		
		/****************************************************/
		map_points_init();
        map_load();
		map_handling("init");
        /**************************************************/
        
		/*------------- Map event function handles --------------*/
		
 		/************************************************/
		
		document.getElementById("area_list").onchange = function() {
			/****Area drop down list handle***/
			/**On each change of dropdown list obtain zoom details***/
			var selection = this.value;	
			send_ajax_get( {% url 'parking:area_get' %}, selection, zoom_area, []);	
		}
 		
 		function OnMapClick(map_point){
 			/*show the position of the click*/
 			info.update('<h4>Map</h4><b> ( '  + map_point.latlng.lat +' , '  + map_point.latlng.lng +' )</b>');
 			/*************************/
 			if( state=="lamp_add" || state=="click_lamp_add") {
 				/******In add lamp mode****/
 				if(!newlamp_marker){
 					/*Marker not on the map, add point*/
	    			newlamp_marker = new L.marker(map_point.latlng,
	        		{
	            		draggable: false
	        		});
	        		lampItems.addLayer(newlamp_marker);
        		}
        		else{
        			/*Marker already on the map, replace point*/
        			newlamp_marker.setLatLng(new L.LatLng(map_point.latlng.lat, map_point.latlng.lng ));
        		}
        		lamp = {
	    				max_range: document.getElementById("max_range_text").value,
	    				lat: map_point.latlng.lat,
	    				lng: map_point.latlng.lng
					};
 				add_circle_marker(lamp);
        		/*update position variable to use it in save*/
        		position = map_point.latlng
        		map_handling("click_lamp_add");
    		}
    		else{
    			/******NOT in add lamp mode****/
    			if(circlemarker && state!="parking_add"){
    				map.removeLayer(circlemarker);
    			}
    			if(state!="init" && state!="lamp_edit" && state!="parking_add" && state!="parking_draw_created" && state!="parking_edit" && state!="parking_edit_draw_created"){
    				map_handling("init");
    			}
    		}
    		
		}
		
		function onParkingClick(parkingclick) {
			_parking = parkingByID[parkingclick.target.id];
			parking_lamp = lampByID[_parking.lamp_location];
			info.update('<h4>Parking</h4><b>ID: '+parkingclick.target.id+'</b><br>'+'<b>At lamp ID: ' + "..loading"  + '<br><b>Occupancy: ' + "..loading"  + '<br />Capacity: ' + "..loading"  + '</b>'+'</b><br><b>Map: ( '  + parkingclick.latlng.lat +' , '  + parkingclick.latlng.lng +' )</b>');    		
    		if( state!="lamp_add" && state!="click_lamp_add" && state!="parking_add" && state!="parking_edit" && state!="lamp_edit" && state!="parking_draw_created" && state!="parking_edit_draw_created") {
	    		$("#map_points").html('Add new lamp to the system');
				map_handling("parking_select");	
				_parking_edit = parkingclick;
				send_ajax_get( {% url 'parking:parking_get' %}, _parking.id, show_parking_details, [_parking, parkingclick]);
	    		
			}
			else if( state=="lamp_add" || state=="click_lamp_add") {
 				/******In add lamp mode****/
 				$("#map_points").html('Click on map to add new lamp to the system and set its max range');
 				if(!newlamp_marker){
 					/*Marker not on the map, add point*/
	    			newlamp_marker = new L.marker(parkingclick.latlng,
	        		{
	            		draggable: false
	        		});
	        		lampItems.addLayer(newlamp_marker);
        		}
        		else{
        			/*Marker already on the map, replace point*/
        			newlamp_marker.setLatLng(new L.LatLng(parkingclick.latlng.lat, parkingclick.latlng.lng ));
        		}
        		lamp = {
	    				max_range: document.getElementById("max_range_text").value,
	    				lat: parkingclick.latlng.lat,
	    				lng: parkingclick.latlng.lng
					};
 				add_circle_marker(lamp);
        		/*update position variable to use it in save*/
        		position = parkingclick.latlng
        		map_handling("click_lamp_add");
        	}
    	}
		
 		function onMarkerDragend(marker) {
 			var lamp;
 			//marker.target.dragging.disable();
        	$("#saveLampEdit").button('option', "disabled", false);
        	position = marker.target.getLatLng();
        	info.update('<h4>New Lamp</h4><b>ID: '+marker.target.lampID+ '</b><br><b>Map: ( '  + position.lat +' , '  + position.lng +' )</b>');
 			$("#lamp_points").html('Save new lamp at selected position');
 			lamp = {
	    				max_range: document.getElementById("max_range_change_text").value,
	    				lat: position.lat,
	    				lng: position.lng
					};
 			add_circle_marker(lamp);
 			
    	}
 		
		function onMarkerClick(marker) {
			_lamp = lampByID[marker.target.lampID];
			info.update('<h4>Lamp</h4><b>ID: '+_lamp.id +'</br>Map: ( '  +  _lamp.lat +' , '  + _lamp.lng +' )' + '<br>Range: ' + _lamp.max_range + '<br>Eth0: ' + _lamp.lamp_number + '</br>Online status: ' + '..loading'  +'</b>');
			send_ajax_get( {% url 'parking:lamp_online_get' %}, _lamp.id, show_lamp_details, [_lamp]);
			if( $("#cancelLamppoints").prop('disabled') ) {
				/***On usual lamp select****/
    			_marker = marker;
        		position = marker.target.getLatLng();
    			map_handling("lamp_select");
    		}
    		add_circle_marker(_lamp);
		}
        /**************************************************/
        
		/*------------AJAX functions ---------------*/

		/***********************************************/
		
		function send_ajax_get(url_address, content_type, callback, args) {
			$.ajax({
				"type": "GET",
				"url": url_address,
				"data": { 'content' : JSON.stringify(content_type) }, 
			    "dataType": "json",
				"success": function(response){
					try {
						args.push([JSON.stringify(response)]);
						if (callback!=0){
							callback.apply(this,args);
						}
					}
					catch(err) {
					    alert(err.message);
					}
					
				},
				error : function(xhr,errmsg,err) {
					alert("server error "+ xhr.status + ": " + xhr.responseText);
				}
			});		
		}
		function send_ajax_post(ContentToSave, data_type) {
			var id;
    		function getCookie(name) {
    			var cookieValue = null;
    			if (document.cookie && document.cookie != '') {
        			var cookies = document.cookie.split(';');
        			for (var i = 0; i < cookies.length; i++) {
            			var cookie = jQuery.trim(cookies[i]);
            			// Does this cookie string begin with the name we want?
            			if (cookie.substring(0, name.length + 1) == (name + '=')) {
                			cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
                			break;
            			}
        			}
    			}
    			return cookieValue;
			}
			$.ajaxSetup({
    			beforeSend: function(xhr, settings) {
        			if (!(/^http:.*/.test(settings.url) || /^https:.*/.test(settings.url))) {
            			// Only send the token to relative URLs i.e. locally.
            			xhr.setRequestHeader("X-CSRFToken", getCookie('csrftoken'));
        			}
    			}
			});
        	$.ajax({
				"type": "POST",
				"url": '{% url 'parking:lamphandle' %}',
				"data": { "content" : JSON.stringify(ContentToSave),
					"content_type" : JSON.stringify(data_type)}, 
                "dataType": "json",
				"beforeSend": function(xhr, settings) {
        			$.ajaxSettings.beforeSend(xhr, settings);
				},
				"success": function(response){
            		if (response["HTTPRESPONSE"] == 1){
            			if(data_type=="edit_lamp" || data_type=="edit_parking"){
            				id=ContentToSave[0].id;
            			}
            			else if(data_type=="add_lamp" || data_type=="add_parking"){
            				id=response["id"];
            			}
            			add_point_to_map(data_type, ContentToSave[0], id);
            			page_reload();
            		}
      				else
      				{
      					alert("error "+response["HTTPRESPONSE"])
                		page_refresh();
      				}
        		},
				error : function(xhr,errmsg,err) {
					alert("server error "+ xhr.status + ": " + xhr.responseText);
				}
			});		
		}
		
        /**************************************************/
        
		/*----------Click handlers on edit fields-----------------*/
		
		/*****************************************************/
		document.getElementById("max_range_change_text").onclick=function(){
			$("#saveLampEdit").button('option', "disabled", false);
		};
		document.getElementById("change_capacity_text").onclick=function(){
			$("#SaveEditParking").button('option', "disabled", false);
		};
		document.getElementById("eth0_edit_div").onclick=function(){
			$("#saveLampEdit").button('option', "disabled", false);
		};
        /**************************************************/
        
		/*----------- SAVE button handles ----------------*/
		
		/**************************************************/
		$("#saveLamp").button({disabled: true}).click(function(){
			var LamppointsToSave = [];
			var lamp_number = document.getElementById("eth0_list").value;
            LamppointsToSave.push({
                lamp_number: lamp_number,
                max_range: document.getElementById("max_range_text").value,
                lat: position.lat,
                lng: position.lng
            });
			send_ajax_post(LamppointsToSave,"add_lamp");
			if (lamp_number=="None" && _lamp.lamp_number!="None"){
				send_ajax_post(_lamp.lamp_number,"set_queue");
			}
			info.update('<h4>Lamp</h4><b>Lamp added!</b>');
		});
		
		$("#saveLampEdit").button({disabled: true}).click(function(){
        	var LamppointsToSave = [];
			var lamp_number = document.getElementById("eth0_list").value;
            LamppointsToSave.push({
                id: _marker.target.lampID,
                lamp_number: lamp_number,
                max_range: document.getElementById("max_range_change_text").value,
                lat: position.lat,
                lng: position.lng
            });
            send_ajax_post(LamppointsToSave,"edit_lamp");
			if (lamp_number=="None" && _lamp.lamp_number!="None"){
				send_ajax_post(_lamp.lamp_number,"set_queue");
			}
			info.update('<h4>Lamp</h4><b>Lamp edited!</b>');
        	
    	});
		
		$("#saveParking").button({disabled: true}).click(function(){
			var ParkingToSave = [];
			var geometry = created_parking.geometry.coordinates[0];
            ParkingToSave.push({
            	lamp_location: _marker.target.lampID,
            	capacity: document.getElementById("capacity_text").value,
                geom: geometry
            });
			//alert(JSON.stringify(created_parking.geometry))
            send_ajax_post(ParkingToSave,"add_parking");
		});
		
		$("#SaveEditParking").button({disabled: true}).click(function(){
			var ParkingToSave = [];
			var geometry = created_parking.geometry.coordinates[0];
            ParkingToSave.push({
            	id: _parking_edit.target.id,
            	lamp_location: _parking_edit.target.lamp_location,
            	capacity: document.getElementById("change_capacity_text").value,
                geom: geometry
            });
            send_ajax_post(ParkingToSave,"edit_parking");
		});
        /**************************************************/
        
		/*------------Button handles ---------------*/
		
		/*************************************************/
		
    	$("#cancelLamppoints").button({disabled: true}).click(function(){
    		if(circlemarker){
    			map.removeLayer(circlemarker);
    		}
    		page_reload();
		});
		$("#refresh").button({disabled: false}).click(function(){
			page_refresh();
		});
		$("#status").button({disabled: false}).click(function(){
			info.update('<h4>Click parking to display occupancy and capacity</h4>'+
			    	'<b>Lamps offline: '+ '..loading'+ '</br>'+
			    	'<b>Lamps with errors pending since online: '+ '..loading'+ '</br>'+
			    	'Lamps eth0 requests pending: '+ '..loading' + '</b>');
			var table = document.getElementById("lamp_status_table");
			table.innerHTML = "";
			var row = table.insertRow(0);			
			var cell1 = row.insertCell(0);
			cell1.innerHTML = "<b>...loading</b>";
			send_ajax_get( {% url 'parking:lamp_register_get' %}, "count", update_info_bar, []);
		});
		
		$("#editLamppoints").button({disabled: true}).click(function(){
    		map_handling("lamp_edit");
		});
		
		$("#addLamp").button({disabled: false}).click(function(){
    		map_handling("lamp_add");
		});
		
		$("#addParking").button({disabled: true}).click(function(){
			map_handling("parking_add");
		});
		
		$("#editParking").button({disabled: true}).click(function(){
			map_handling("parking_edit");
		});
        /**************************************************/
        
		/*--------- Supplementary Functions ------------------*/
		
		/*********************************************/
		
		function page_refresh() {
    		location.reload();
    		document.getElementById("area_list").value='World';
		}
		
		function page_reload() {
    		parkingItems.clearLayers();
    		lampItems.clearLayers();
    		drawnItems.clearLayers();
        	map_load();
			map_handling("init");
			draw_control("disable");
	        draw_control("init");
		}
		
		function lamp_div_options(div_name){
			var id = null
			if(_lamp){
				id = _lamp.id
			}
			document.getElementById(div_name).innerHTML += "</br><b>Eth0: </b><select id=" + "eth0_list" + " ></select></br><b>ID: </b>"+id;
			send_ajax_get( {% url 'parking:lamp_register_get' %}, "show", reg_lamps_details, []);
			
		}
		function reg_lamps_details(response) {
			response = JSON.parse(response);
			try {
	    		if (response["HTTPRESPONSE"] == 1){
					reg_lamps = JSON.parse(response["lamp_register_points"]);
					var select = document.getElementById("eth0_list");
					if(_lamp){
						lamp_number = _lamp.lamp_number;
					}
					else{
						lamp_number ='None'
					}
					if(lamp_number!='None'){
						select.options[select.options.length] = new Option(lamp_number, lamp_number);
						select.options[select.options.length] = new Option('None', 'None');
					}
					else{
						select.options[select.options.length] = new Option(lamp_number, lamp_number);
					}
					
					for(index in reg_lamps) {
    					select.options[select.options.length] = new Option(reg_lamps[index], reg_lamps[index]);
					}
				}
				else
				{
					alert("error "+response["HTTPRESPONSE"]);
				}
			}
			catch(err) {
			    alert(err.message);
			}
		}
		function add_circle_marker(lamp){
    		if(circlemarker){
    			map.removeLayer(circlemarker);
    		}
        	circlemarker = L.circle([lamp.lat, lamp.lng],lamp.max_range).addTo(map);
		}
		
		function add_point_to_map(op_type, data_to_save, id){
			if(op_type=="edit_lamp"){
				lampByID[id].max_range = data_to_save.max_range;
				lampByID[id].lamp_number = data_to_save.lamp_number;
				lampByID[id].lng = data_to_save.lng;
				lampByID[id].lat = data_to_save.lat;
				add_circle_marker(data_to_save);
			}
			else if(op_type=="edit_parking"){
				parkingByID[id].lamp_location = data_to_save.lamp_location;
				parkingByID[id].capacity = data_to_save.capacity;
				parkingByID[id].geom = {"type":"Polygon", "coordinates":[data_to_save.geom]};
			}
			else if(op_type=="add_lamp"){
				lampByID[id] = {
						id: id,
	    				max_range: data_to_save.max_range,
	    				lamp_number: data_to_save.lamp_number,
	    				lat: data_to_save.lat,
	    				lng: data_to_save.lng
					};
				add_circle_marker(data_to_save);
			}
			else if(op_type=="add_parking"){
				parkingByID[id] = {
						id: id,
	    				lamp_location: data_to_save.lamp_location,
	    				capacity: data_to_save.capacity,
	    				geom: {"type":"Polygon", "coordinates":[data_to_save.geom]}
					};
			}
		
		}
		
		function show_lamp_details(_lamp,response){
			response = JSON.parse(response);
			try {
	    		if (response["HTTPRESPONSE"] == 1){
	    			info.update('<h4>Lamp</h4><b>ID: '+_lamp.id +'</br>Map: ( '  +  _lamp.lat +' , '  + _lamp.lng +' )' + '<br>Range: ' + _lamp.max_range + '<br>Eth0: ' + _lamp.lamp_number + '</br>Online status: ' + response['online']  +'</b>');
			    	
				}
				else
				{
					alert("error "+response["HTTPRESPONSE"]);
				}
			}
			catch(err) {
			    alert(err.message);
			}
		}
		
		function show_parking_details(_parking,parkingclick,response){
			response = JSON.parse(response);
			try {
	    		if (response["HTTPRESPONSE"] == 1){
			    	info.update('<h4>Parking</h4><b>ID: '+_parking.id+'</b><br>'+'<b>At lamp ID: ' + _parking.lamp_location  + '<br><b>Occupancy: ' + response["parking"]+'<br />Capacity: ' + _parking.capacity +'</b>'+'</b><br><b>Map: ( '  + parkingclick.latlng.lat +' , '  + parkingclick.latlng.lng +' )</b>');
				}
				else
				{
					alert("error "+response["HTTPRESPONSE"]);
				}
			}
			catch(err) {
			    alert(err.message);
			}
		}
		
		
		function zoom_to_lamp(row){
			var id = row.getElementsByTagName("td")[0].innerHTML;
			var selected_lamp = lampByID[id];
			map.setView([selected_lamp.lat,selected_lamp.lng],18);
			info.update('<h4>Lamp</h4><b>ID: '+selected_lamp.id +'</br>Map: ( '  +  selected_lamp.lat +' , '  + selected_lamp.lng +' )' + '<br>Range: ' + selected_lamp.max_range + '<br>Eth0: ' + selected_lamp.lamp_number + '</br>Online status: ' + '..loading'  +'</b>');
			send_ajax_get( {% url 'parking:lamp_online_get' %}, selected_lamp.id, show_lamp_details, [selected_lamp]);
    		add_circle_marker(selected_lamp);
    		var row_cnd = row.getElementsByTagName("td")[1].innerHTML;
    		if(row_cnd != "OFFLINE" && row_cnd != "ONLINE"){
    			send_ajax_post(selected_lamp.lamp_number,"reset_sig");
    			alert("Restarts microcontroller...");
    		}
		}

		var TableBackgroundNormalColor = "#ffffff";
		var TableBackgroundMouseoverColor = "#C0C0C0";

		function ChangeBackgroundColor(row) { row.style.backgroundColor = TableBackgroundMouseoverColor; }
		function RestoreBackgroundColor(row) { row.style.backgroundColor = TableBackgroundNormalColor; }
		
		function update_info_bar(response){
			var row, table, cell1, cell2, lamp_errors;
			response = JSON.parse(response);
			try {
	    		if (response["HTTPRESPONSE"] == 1){
	    			
			    	info.update('<h4>Click parking to display occupancy and capacity</h4>'+
			    	'<b>Lamps offline: '+ response["lamp_register_online_count"]+ '</br>'+
			    	'<b>Lamps with errors pending since online: '+ response["lamp_register_error_count"] + '</br>'+
			    	'Lamps eth0 requests pending: '+ response["lamp_register_wait_count"]+ '</b>');
			    	
					table = document.getElementById("lamp_status_table");
					table.innerHTML = "";
					document.getElementById("lamp_status").className = "lamp_status";
					lamp_errors = response["lamp_error_dict"];
					row = table.insertRow();			
					cell1 = row.insertCell(0);		
					cell2 = row.insertCell(1);
					cell2.style.width = '100%';
					cell1.innerHTML = "<b>Lamp ID</b>";
					cell2.innerHTML = "<b>Status</b>";
					for (var id in lamp_errors) {
						row = table.insertRow();
						row.onclick = function(){ zoom_to_lamp(this); };
						row.onmouseover=function(){ ChangeBackgroundColor(this); }; 
						row.onmouseout=function(){ RestoreBackgroundColor(this); };
						cell1 = row.insertCell(0);		
						cell2 = row.insertCell(1);
						cell1.innerHTML = String(id);
						cell2.innerHTML = String(lamp_errors[id]);
					}
			    	
				}
				else
				{
					alert("error "+response["HTTPRESPONSE"]);
				}
			}
			catch(err) {
			    alert(err.message);
			}
		}
		
		function zoom_area(response){
			response = JSON.parse(response);
			try {
				if (response["HTTPRESPONSE"] == 1){
					map.setView([response["lat"],response["lon"]],response["area_type"]);
					info.update('<h4>Zoom</h4><b> ( '  + response["lat"] +' , '  + response["lon"] +' )</b>');
				}
				else
				{
					alert("error "+response["HTTPRESPONSE"]);
				}
			}
			catch(err) {
			    alert(err.message);
			}
		}
        /**************************************************/
        
		/*--------Map points initialization functions-------------------*/
		
		/***************************************************/
		
		function map_points_init(){
			{% for lamp in lamp_points %}
				{% if lamp %}
					lampByID["{{ lamp.id }}"] = {
						id: "{{ lamp.id }}",
	    				max_range: "{{lamp.max_range}}",
	    				lamp_number: "{{lamp.lamp_number}}",
	    				lat: {{ lamp.geom.centroid.y }},
	    				lng: {{ lamp.geom.centroid.x }}
					};
				{% endif %}
			{% endfor %}
			
			{% for parking in parking_points %}
				{% if parking %}
					parkingByID["{{ parking.id }}"] = {
						id: "{{ parking.id }}",
	    				lamp_location: "{{parking.lamp_location}}",
	    				capacity: {{ parking.capacity }},
	    				geom: {{ parking.geom.geojson|safe }}
					};
				{% endif %}
			{% endfor %}
		}
		
		function map_load() {
    		/****Initialize lamps at the map ********/
			for (var i in lampByID) {
	    			_lamp = lampByID[i];
	    			/****Add marker to the map and assign lampID parameter to it****/
					marker = L.marker([_lamp.lat, _lamp.lng],
	        		{
	            		draggable: false
	        		});
	        		marker.lampID = _lamp.id;
	        		
					/*****Add events handlers to the markers****/
	    			marker.on('click', onMarkerClick);
	        		marker.on('dragend', onMarkerDragend);
	        		lampItems.addLayer(marker);
			}	
			map.addLayer(lampItems);
			
			/****Initialize parking at the map ********/
			for (var i in parkingByID) {
	 				/*** For each parking add polygon****///
	    			_parking = parkingByID[i];
					polygon = L.geoJson(_parking.geom);
					polygon.setStyle({color: "green"});
					polygon.lamp_location = _parking.lamp_location;
					polygon.id = _parking.id;
	        		polygon.on('click', onParkingClick);
					//alert(JSON.stringify(_parking.geom))
					parkingItems.addLayer(polygon);
			}	
			map.addLayer(parkingItems);
		}
        /**************************************************/
        
		/*---------Draw functions ------------------*/
		
		/**************************************/
		
		function draw_editstart(e) {
    		$("#saveParking").button('option', 'disabled', true);
		}
		
		function draw_editstop(e) {
    		
		}
		
		function draw_drawstop(e) {
			draw_control("disable");
			draw_control("edit");
			
		}
		
		function draw_created(e) {
            var layer = e.layer;
                
            created_parking = layer.toGeoJSON();
            drawnItems.clearLayers();
            drawnItems.addLayer(layer);
            if (state=="parking_edit" || state=="parking_edit_draw_created"){
            	map_handling("parking_edit_draw_created");
            }
            else{
            	map_handling("parking_draw_created");
            }
            
		}
		
		function draw_edited(e) {
            e.layers.eachLayer(function(layer) {
    			draw_created({ layer: layer });
  			});
		}
		
		function draw_control(control_type) {
			if(control_type=="add"){
        		map.addControl(drawControl);
			}
			else if(control_type=="disable" && drawControl){
				map.removeControl(drawControl);
			}
			else if(control_type=="edit"){
				drawControl = new L.Control.Draw({
	           		edit: {
	                	featureGroup: drawnItems
	            	},
	            	draw: {
	    				polygon: false,
	    				polyline: false,
	    				rectangle: false,
	    				circle: false,
	    				marker: false
	  				}
        		});
				map.addControl(drawControl);
        	}
        	else if(control_type=="init"){
	        	drawControl = new L.Control.Draw({
		           	edit: {
		                featureGroup: drawnItems
		            },
		            draw: {
		    			polygon: true,
		    			polyline: false,
		    			rectangle: false,
		    			circle: false,
		    			marker: false
		  			}
        		});
        	}
		}
        /**************************************************/
        
		/*--------Button click handles for the map-------------------*/
		
		/*******************************/
		
		function map_handling(status) {
			$("#saveLampEdit").button('option', "disabled", true);
        	$("#cancelLamppoints").button('option', "disabled", true);
        	$("#editLamppoints").button('option', "disabled", true);
        	$("#addLamp").button('option', "disabled", true);
        	$("#saveLamp").button('option', "disabled", true);
        	$("#addParking").button('option', "disabled", true);
        	$("#saveParking").button('option', "disabled", true);
        	$("#SaveEditParking").button('option', "disabled", true);
 			$("#editParking").button('option', "disabled", true);
 			document.getElementById("changelamp").style.display="None";
        	document.getElementById("deletelamp").style.display="None";
        	document.getElementById("deleteparking").style.display="None";
        	document.getElementById("changeparking").style.display="None";
        	
        	document.getElementById("lamp_edit_div").style.display="None";
        	document.getElementById("parking_edit_div").style.display="None";
        	document.getElementById("lamp_add_div").style.display="None";
        	
			document.getElementById("max_range_div").style.display="None";
			document.getElementById("capacity_div").style.display="None";
			document.getElementById("change_capacity_div").style.display="None";
			document.getElementById("eth0_add_div").style.display="None";
			document.getElementById("eth0_edit_div").style.display="None";
			document.getElementById("eth0_edit_div").innerHTML = "";
			if(status!="click_lamp_add"){
				document.getElementById("eth0_add_div").innerHTML = "";
			}
        	/*** Update state variable for map handling***/
 			state=status;
 			
			if (status=="init"){
				$("#lamp_points").html('No lamp selected to edit or add new parking');
				$("#map_points").html('Add new lamp to the system');
				$("#parking_points").html('No parking selected to edit');
        		document.getElementById("lamp_edit_div").style.display="inherit";
        		document.getElementById("parking_edit_div").style.display="inherit";
        		document.getElementById("lamp_add_div").style.display="inherit";
        		$("#addLamp").button('option', "disabled", false);
			}
			else if (status=="lamp_edit"){
    			_marker.target.dragging.enable();
				$("#lamp_points").html('Change selected lamp position or edit other settings');
        		document.getElementById("changelamp").href = '../admin/parking/lamp/'+_marker.target.lampID+'/';
        		document.getElementById("deletelamp").href = '../admin/parking/lamp/'+_marker.target.lampID+'/delete/';
        		document.getElementById("deletelamp").style.display="inline";
        		document.getElementById("changelamp").style.display="inline";
        		$("#cancelLamppoints").button('option', "disabled", false);
        		document.getElementById("lamp_edit_div").style.display="inherit";
        		document.getElementById("parking_edit_div").style.display="None";
        		document.getElementById("lamp_add_div").style.display="None";
				document.getElementById("eth0_edit_div").style.display="inline";
				lamp_div_options("eth0_edit_div");
			}
			else if (status=="lamp_add"){
				_lamp = null;
				/**** init lamp add writing, cancel button and max range division, disable add and save lamp buttons****/
    			$("#map_points").html('Click on map to add new lamp to the system and set its max range');
				document.getElementById("max_range_div").style.display="inline";
        		$("#cancelLamppoints").button('option', "disabled", false);
        		$("#addLamp").button('option', "disabled", true);
        		$("#saveLamp").button('option', "disabled", true);
        		document.getElementById("lamp_add_div").style.display="inherit";
        		document.getElementById("lamp_edit_div").style.display="None";
        		document.getElementById("parking_edit_div").style.display="None";
				document.getElementById("eth0_add_div").style.display="inline";
				lamp_div_options("eth0_add_div");
			}
			else if (status=="parking_add"){
				draw_control("add");
    			$("#map_points").html('Add parking using toolbox on the map');
    			$("#cancelLamppoints").button('option', 'disabled', false);
				document.getElementById("capacity_div").style.display="inline";
        		document.getElementById("lamp_edit_div").style.display="inherit";
        		document.getElementById("parking_edit_div").style.display="None";
        		document.getElementById("lamp_add_div").style.display="None";
			}
			else if (status=="parking_edit"){
    			$("#cancelLamppoints").button('option', 'disabled', false);
				_parking_edit.target.setStyle({color: "DeepPink"});
            	drawnItems.clearLayers();
           		drawnItems.addLayer(_parking_edit.layer);
				draw_control("edit");
				document.getElementById("change_capacity_div").style.display="inline";
        		document.getElementById("deleteparking").style.display="inline";
				document.getElementById("deleteparking").href = '../admin/parking/parking/'+_parking.id+'/delete/';
        		document.getElementById("changeparking").style.display="inline";
				document.getElementById("changeparking").href = '../admin/parking/parking/'+_parking.id+'/';
        		document.getElementById("lamp_add_div").style.display="None";
        		document.getElementById("lamp_edit_div").style.display="None";
        		document.getElementById("parking_edit_div").style.display="inherit";
			}
			else if (status=="click_lamp_add"){
    			$("#saveLamp").button('option', 'disabled', false);
    			$("#cancelLamppoints").button('option', 'disabled', false);
        		$("#addLamp").button('option', "disabled", true);
        		document.getElementById("lamp_add_div").style.display="inherit";
        		document.getElementById("lamp_edit_div").style.display="None";
        		document.getElementById("parking_edit_div").style.display="None";
				document.getElementById("max_range_div").style.display="inline";
				document.getElementById("eth0_add_div").style.display="inline";
			}
			else if (status=="lamp_select"){
        		document.getElementById("lamp_edit_div").style.display="inherit";
        		document.getElementById("parking_edit_div").style.display="None";
        		document.getElementById("lamp_add_div").style.display="inherit";
        		$("#editLamppoints").button('option', "disabled", false);
        		$("#addLamp").button('option', "disabled", false);
        		$("#addParking").button('option', "disabled", false);
    			$("#map_points").html('No map selection for lamp/parking');
 				$("#lamp_points").html('Edit selected lamp or add new parking');
			}
			else if (status=="parking_draw_created"){
    			$("#saveParking").button('option', 'disabled', false);
    			$("#cancelLamppoints").button('option', 'disabled', false);
				document.getElementById("capacity_div").style.display="inline";
        		document.getElementById("lamp_edit_div").style.display="inherit";
        		document.getElementById("parking_edit_div").style.display="None";
        		document.getElementById("lamp_add_div").style.display="None";
			}
			else if (status=="parking_edit_draw_created"){
    			$("#SaveEditParking").button('option', 'disabled', false);
    			$("#cancelLamppoints").button('option', 'disabled', false);
				document.getElementById("change_capacity_div").style.display="inline";
        		document.getElementById("deleteparking").style.display="inline";
				document.getElementById("deleteparking").href = '../admin/parking/parking/'+_parking.id+'/delete/';
        		document.getElementById("changeparking").style.display="inline";
				document.getElementById("changeparking").href = '../admin/parking/parking/'+_parking.id+'/';
        		document.getElementById("lamp_edit_div").style.display="None";
        		document.getElementById("parking_edit_div").style.display="inherit";
        		document.getElementById("lamp_add_div").style.display="None";
			}
			else if (status=="parking_select"){
        		$("#editParking").button('option', "disabled", false);
				$("#lamp_points").html('No lamp selected to edit');
				$("#parking_points").html('Edit selected parking');
        		document.getElementById("lamp_edit_div").style.display="None";
        		document.getElementById("parking_edit_div").style.display="inherit";
        		document.getElementById("lamp_add_div").style.display="inherit";
        		$("#addLamp").button('option', "disabled", false);
			}
		}
        /**************************************************/
        
		/*-------------THE END--------------*/		
		
	});


