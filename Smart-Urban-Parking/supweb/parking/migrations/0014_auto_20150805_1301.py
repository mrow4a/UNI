# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import datetime


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0013_remove_lamp_register_queue_lamp_request_ip'),
    ]

    operations = [
        migrations.AddField(
            model_name='lamp',
            name='online_timestamp',
            field=models.DateTimeField(default=datetime.datetime.now, blank=True),
        ),
        migrations.AddField(
            model_name='lamp_register_queue',
            name='error_status',
            field=models.CharField(default=b'None', max_length=256, db_column=b'error_status', blank=True),
        ),
        migrations.AddField(
            model_name='lamp_register_queue',
            name='online_status',
            field=models.CharField(default=b'Offline', max_length=256, db_column=b'online_status', blank=True),
        ),
    ]
