# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0020_auto_20150809_1543'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='ble_tag',
            name='timestamp',
        ),
        migrations.RemoveField(
            model_name='lamp',
            name='online_timestamp',
        ),
        migrations.AddField(
            model_name='ble_tag',
            name='lamp_location',
            field=models.ForeignKey(to='parking.Lamp', null=True),
        ),
    ]
