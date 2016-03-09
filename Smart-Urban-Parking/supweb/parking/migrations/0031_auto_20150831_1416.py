# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0030_lamp_software'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='lamp_software',
            name='ble_soft',
        ),
        migrations.AlterField(
            model_name='lamp_software',
            name='car_detection_soft',
            field=models.FileField(upload_to=b'software/%Y/%m/%d'),
        ),
    ]
