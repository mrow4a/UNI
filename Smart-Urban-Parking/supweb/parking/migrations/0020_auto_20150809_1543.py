# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import datetime


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0019_remove_lamp_online_timestamp'),
    ]

    operations = [
        migrations.AddField(
            model_name='lamp',
            name='online_timestamp',
            field=models.DateTimeField(default=datetime.datetime.utcnow, blank=True),
        ),
        migrations.AlterField(
            model_name='ble_tag',
            name='timestamp',
            field=models.DateTimeField(),
        ),
    ]
