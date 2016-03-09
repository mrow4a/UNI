# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import django.utils.timezone


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0029_lamp_error_reset_signal'),
    ]

    operations = [
        migrations.CreateModel(
            name='Lamp_Software',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('timestamp', models.DateTimeField(default=django.utils.timezone.now, blank=True)),
                ('ble_soft', models.FileField(upload_to=b'/software')),
                ('car_detection_soft', models.FileField(upload_to=b'/software')),
            ],
        ),
    ]
